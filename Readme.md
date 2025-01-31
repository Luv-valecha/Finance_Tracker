# Finance Tracker

A full-stack personal finance management application combining a C++ backend with a Node.js/Express web interface using RESTful APIs. Track expenses, manage budgets, and get personalized spending recommendations.

## Screenshots

![Screenshot 1](https://github.com/Luv-valecha/Finance_Tracker/blob/main/project_ss/ss1.png?raw=true)
![Screenshot 2](https://github.com/Luv-valecha/Finance_Tracker/blob/main/project_ss/ss2.png?raw=true)
![Screenshot 3](https://github.com/Luv-valecha/Finance_Tracker/blob/main/project_ss/ss3.png?raw=true)
![Screenshot 4](https://github.com/Luv-valecha/Finance_Tracker/blob/main/project_ss/ss4.png?raw=true)
![Screenshot 5](https://github.com/Luv-valecha/Finance_Tracker/blob/main/project_ss/ss5.png?raw=true)

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

### Frontend 
- HTML/CSS
- Javascript
- REST APIs

## Installation

1. Clone the repository
```bash
git clone https://github.com/Luv-valecha/Finance_Tracker.git
cd financetracker
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

4. Create environment file
```bash
# .env
PORT=3000
```

5. Start the server
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
│   └── Statistics.cpp
├── frontend/
│   ├── public/
│   │   ├── index.html
│   │   ├── register.html 
│   │   ├── dashboard.html
|   |   ├── css/
|   |   |   ├──images/
|   |   |   └──style.css
|   |   └──js/
|   |   |   ├──app.js
|   |   |   └──utils.js
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

### Prerequisites
- Node.js
- C++ compiler
- npm

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
