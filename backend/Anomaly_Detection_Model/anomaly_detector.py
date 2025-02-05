# -----------------BACKEND SERVER TO DETETCT ANOMALY FOR FINANCELY-------------------------

from flask import Flask, request, jsonify
import pickle
import pandas as pd
from sklearn.preprocessing import StandardScaler, OneHotEncoder
from datetime import datetime
from flask_cors import CORS

# load the model,scaler and encoder saved during training
modelfile= open("Isolation_Forest/anomaly_detection_model.pkl",'rb')
model=pickle.load(modelfile)
encoderfile= open("encoder.pkl", "rb")
ohe=pickle.load(encoderfile)
scalerfile=open("scaler.pkl", "rb")
scaler=pickle.load(scalerfile)

# feature engineering required by the model as done in training
def feature_engineering(df):
    df['date'] = df['date'].apply(lambda x: datetime.strptime(x, '%Y-%m-%d'))
    df['hour'] = df['date'].dt.hour
    df['day_of_week'] = df['date'].dt.dayofweek
    df['time_of_month'] = df['date'].dt.day
    df['is_weekend'] = df['day_of_week'].apply(lambda x: 1 if x >= 5 else 0)

    # df['Type'] = df['Type'].map({"Debit": 0, "Credit": 1})

    # Normalize 'amount'
    df['scaled_amount'] = scaler.transform(df[['amount']])

    categories = ["Income",	"Rent", "Food",	"Shopping",	"Entertainment", "Personal", "Health", "Travel", "Miscellaneous"]
    category_reshaped = df[['category']].values.reshape(-1, 1)

    # Apply OneHotEncoder using the saved encoder
    category_encoded = ohe.transform(category_reshaped)

    encoded_df = pd.DataFrame(category_encoded, columns=categories)

    df = pd.concat([df, encoded_df], axis=1).drop(columns=['category'])

    # mark all categories except the transaction category as 0
    for cat in categories:
        if cat not in df.columns:
            df[cat] = 0

    # Select features for training
    features = ['scaled_amount', 'hour', 'day_of_week', 'time_of_month', 'is_weekend']
    features.extend(categories)
    X = df[features]

    return X

# Backend app
app = Flask(__name__)

CORS(app)

# api
@app.route('/detect_anomaly',methods=['POST'])
def detect_anamoly():
    data=request.json
    print(f'got data: {data}')
    transaction=pd.DataFrame([data])

    X=feature_engineering(transaction)

    anamoly=model.predict(X)
    result= "Normal" if anamoly[0]==1 else "Anomaly"

    return jsonify({'prediction':result})


if __name__=='__main__':
    app.run(debug=True, port=5001)

