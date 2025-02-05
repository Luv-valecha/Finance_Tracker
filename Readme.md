# Finance Tracker

A full-stack personal finance management application combining a C++ backend with a Node.js/Express web interface using RESTful APIs. Track expenses, manage budgets, get personalized spending recommendations, and with anomaly detection to flag unusual transactions.

## Screenshots

![Screenshot 1](https://github.com/Luv-valecha/Finance_Tracker/blob/main/project_ss/ss1.png?raw=true)
![Screenshot 2](https://github.com/Luv-valecha/Finance_Tracker/blob/main/project_ss/ss2.png?raw=true)
![Screenshot 3](https://github.com/Luv-valecha/Finance_Tracker/blob/main/project_ss/ss3.png?raw=true)
![Screenshot 4](https://github.com/Luv-valecha/Finance_Tracker/blob/main/project_ss/ss4.png?raw=true)
![Screenshot 5](https://github.com/Luv-valecha/Finance_Tracker/blob/main/project_ss/ss5.png?raw=true)
![Screenshot 6](https://github.com/Luv-valecha/Finance_Tracker/blob/main/project_ss/ss6.png?raw=true)

## Navigation
- [Features](#features)
- [Tech Stack](#tech-stack)
- [Installation](#installation)
- [API Endpoints](#api-endpoints)
- [Project Structure](#project-structure)
- [Usage](#usage)
- [Prerequisites](#prerequisites)
- [Security](#security)
- [Contributing](#contributing)
- [Support](#support)

## Features

### Core Features
- User registration and authentication
- Add and track transactions 
- Set budgets by category
- View spending analytics
- Get personalized recommendations
- Category-wise expense breakdown
- **Anomaly Detection**: Detects unusual spending patterns and alerts the user for confirmation before adding suspicious transactions with an achieved accuracy of 94.63% .

### Available Categories
- Food
- Shopping 
- Entertainment
- Personal
- Health
- Travel
- Rent
- Income
- Miscellaneous

## Tech Stack

### Backend
- C++ core engine
- File-based data persistence
- Custom data structures (doubly linked lists)
- Priority queue for recommendations
- Node.js
- Express.js
- JWT (JSON Web Tokens) for user authentication and authorization
- **Python Backend** for Anomaly Detection:
  - Isolation Forest Model
  - Flask API for anomaly detection

### Frontend 
- HTML/CSS
- Javascript
- REST APIs

## Installation

1. Clone the repository
```bash
git clone https://github.com/Luv-valecha/Finance_Tracker.git
cd Finance_Tracker
```

2. Install Node.js dependencies
```bash
npm install
```

3. Compile C++ backend
```bash
cd .\backend\
g++ -o finance_tracker main.cpp Login_system.cpp Transactions.cpp post_data.cpp get_data.cpp BudgetNRecommender.cpp Statistics.cpp
```

4. Set up the Anomaly Detection Python server:

- Navigate to the "Anomaly_Detection_Model" folder:
```bash
cd ./backend/Anomaly_Detection_Model
```

- Install required Python dependencies (ensure Python 3.x is installed):
```bash
Install required Python dependencies (ensure Python 3.x is installed):
```

- Run the Python server:
```bash
python anomaly_detector.py
```

This will start a Flask server that listens for anomaly detection requests.


5. Create environment file
```bash
# .env
PORT=3000
```

6. Start the Node.js server
```bash
cd .\frontend\
node server.js
```

## API Endpoints

### Authentication
```bash
POST /api/register - Register new user
POST /api/login - Login user (JWT generated on success)
GET /api/logout - Logout user (invalidate JWT on client-side)
```

### Transactions
```bash
POST /api/add-transaction - Add new transaction
GET /api/transactions - Get all transactions
GET /api/cattransactions - Get transactions by category
GET /api/daterangetransactions - Get transactions by date range
POST /api/detect-anomaly - Detect anomalies in a new transaction
```

### Budget & Analytics
```bash
POST /api/setbudget - Set category budgets
GET /api/categorywisespend - Get spending breakdown
GET /api/getrecommendation - Get spending recommendations
```

## Project Structure

```bash
├── backend/
│   ├── main.cpp
│   ├── Login_system.cpp/h  
│   ├── Transactions.cpp/h
│   ├── Transhist.h
│   ├── post_data.cpp
│   ├── get_data.cpp
│   ├── BudgetNRecommender.cpp
│   ├── Statistics.cpp
│   └── Anomaly_Detection_Model/
│       ├── anomaly_detector.py
│       ├── requirements.txt
│       ├── encoder.pkl
│       ├── scaler.pkl
│       └── anomaly_detection_model.pkl
├── frontend/
│   ├── public/
│   │   ├── index.html
│   │   ├── register.html 
│   │   ├── dashboard.html
│   │   ├── css/
│   │   │   ├──images/
│   │   │   └──style.css
│   │   └──js/
│   │   │   ├──app.js
│   │   │   └──utils.js
│   ├── routes/
│   │   └── api.js
│   └── server.js
├── package.json
└── README.md
```

## Usage

1. Register a new account at `/register`

2. Login at `/login`
   - JWT token will be returned upon successful login. Store it securely (e.g., in a cookie or localStorage)

3. Add transactions:
   - Enter date
   - Enter amount
   - Select category
   - Add description

4. View transactions:
   - See all transactions
   - Filter by category
   - Filter by type
   - Filter by date range

5. Manage budget:
   - Set category budgets
   - Track spending vs budget
   - View recommendations

6. Anomaly Detection:
   - When a user adds a new transaction, the system sends the transaction data to the Python backend.
   - The Python server uses an Isolation Forest model, trained on the user’s past transactions, to detect anomalies in the new transaction.
   - If an anomaly is detected, the user is alerted to confirm the transaction details.
   - The transaction is either confirmed and added to the system or rejected based on the user's input.
   - Achieved an accuracy of 94.63%
   - **Note:** To use the anomaly detection feature, ensure the Python server is running separately.

### Prerequisites
- Node.js
- C++ compiler
- npm
- Python 3.x
- Required Python dependencies (pip install -r requirements.txt)

## Security
- Password hashing
- JWT authentication (using tokens for secure user authentication and session management)
- Input validation
- Secure file operations

## Contributing
1. Fork the repo
2. Create feature branch
3. Commit changes
4. Push to branch
5. Open pull request

## Support
For issues or questions, please open an issue in the repository.
