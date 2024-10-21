const express = require('express');
const { exec } = require('child_process');
const path = require('path');
const session = require('express-session');

const app = express();
const port = 3000;

app.use(express.json());
app.use(express.urlencoded({ extended: true }));

app.use(express.static(path.join(__dirname, 'public'), {
  setHeaders: (res, filePath) => {
    if (path.extname(filePath) === '.css') {
      res.setHeader('Content-Type', 'text/css');
    }
  }
}));

// Add session middleware
app.use(session({
  secret: 'your-secret-key',
  resave: false,
  saveUninitialized: true,
}));

// // Middleware to check if user is logged in
// const isAuthenticated = (req, res, next) => {
//   if (req.session.isAuthenticated) {
//     next();
//   } else {
//     res.redirect('/login');
//   }
// };

const execPath = path.resolve(__dirname, '../backend/finance_tracker');

// Serve the login page
app.get('/login', (req, res) => {
  res.sendFile(path.join(__dirname, 'public', 'index.html'));
});

// Serve the register page
app.get('/register', (req, res) => {
  res.sendFile(path.join(__dirname, 'public', 'register.html'));
});

// Serve the main app page
app.get('/app', (req, res) => {
  res.sendFile(path.join(__dirname, 'public', 'dashboard.html'));
});

// Redirect root to login
app.get('/', (req, res) => {
  res.redirect('/login');
});

// const path = require('path');

// Handle user registration
app.post('/api/register', (req, res) => {
  const { username, password } = req.body;
  const execPath = path.resolve(__dirname, '../backend/finance_tracker');
  exec(`"${execPath}" register "${username}" "${password}"`, (error, stdout, stderr) => {
    if (error) {
      console.error(`Exec error: ${error.message}`);
      return res.status(500).json({ error: error.message });
    }
    if (stderr) {
      console.error(`Stderr: ${stderr}`);
      return res.status(500).json({ error: stderr });
    }
    if (stdout.trim() === "User registered successfully") {
      return res.json({ message: "User registered successfully", redirect: "/login" });
    } else {
      console.log("Unexpected output:", stdout.trim());
      return res.status(400).json({ message: stdout.trim() });
    }
  });
});

// Handle user login
app.post('/api/login', (req, res) => {
  const { username, password } = req.body;
  const execPath = path.resolve(__dirname, '../backend/finance_tracker');
  exec(`"${execPath}" login "${username}" "${password}"`, (error, stdout, stderr) => {
    if (error) {
      console.error(`Exec error: ${error.message}`);
      return res.status(500).json({ error: error.message });
    }
    if (stderr) {
      console.error(`Stderr: ${stderr}`);
      return res.status(500).json({ error: stderr });
    }
    const firstLine = stdout.split('\n')[0].trim();  // Get the first line of stdout and trim it
    // console.log(`First line of stdout: ${firstLine}`);
    
    if (firstLine === "Login successful") {
      return res.json({ message: "Login successful", username: username, redirect: `/app?username=${username}` });
    } else {
      console.log("Unexpected output:", firstLine);
      return res.status(401).json({ message: firstLine });
    }
  });
});


// Handle logout
app.get('/api/logout', (req, res) => {
  req.session.destroy((err) => {
    if (err) {
      res.status(500).json({ error: "Could not log out" });
    } else {
      res.json({ message: "Logged out successfully" });
    }
  });
});

// Handle adding a new transaction
app.post('/api/add-transaction', (req, res) => {
  const { date, amount, category, description, username } = req.body;
  const execPath = path.resolve(__dirname, '../backend/finance_tracker');
  exec(`"${execPath}" add_transaction "${username}" "${date}" ${amount} "${category}" "${description.replace(/"/g, '\\"')}"`, (error, stdout, stderr) => {
    if (error) {
      console.error(`Exec error: ${error.message}`);
      return res.status(500).json({ error: error.message });
    }
    if (stderr) {
      console.error(`Stderr: ${stderr}`);
      return res.status(500).json({ error: stderr });
    }
    res.json({ message: stdout.trim() });
  });
});



// Handle getting all transactions
app.get('/api/transactions', (req, res) => {
  const { username } = req.query;
  const execPath = path.resolve(__dirname, '../backend/finance_tracker');
  exec(`"${execPath}" get_transactions "${username}"`, (error, stdout, stderr) => {
    if (error) {
      console.error(`Exec error: ${error.message}`);
      return res.status(500).json({ error: error.message });
    }
    if (stderr) {
      console.error(`Stderr: ${stderr}`);
      return res.status(500).json({ error: stderr });
    }
    // console.log(`stdout: ${stdout}`); // Log stdout to ensure all transactions are fetched
    const transactions = stdout.split('\n').filter(line => line.trim() !== '');
    res.json({ transactions: transactions });
  });
});

// Handle getting category-wise transactions
app.get('/api/cattransactions', (req, res) => {
  const { username, category } = req.query;
  const execPath = path.resolve(__dirname, '../backend/finance_tracker');
  exec(`"${execPath}" get_category_transactions "${username}" "${category}"`, (error, stdout, stderr) => {
    if (error) {
      console.error(`Exec error: ${error.message}`);
      return res.status(500).json({ error: error.message });
    }
    if (stderr) {
      console.error(`Stderr: ${stderr}`);
      return res.status(500).json({ error: stderr });
    }
    // console.log(`stdout: ${stdout}`); // Log stdout to ensure all transactions are fetched
    const transactions = stdout.split('\n').filter(line => line.trim() !== '');
    res.json({ transactions: transactions });
  });
});

// Handle getting transactions within a date range
app.get('/api/daterangetransactions', (req, res) => {
  const { username, from, to } = req.query;
  const execPath = path.resolve(__dirname, '../backend/finance_tracker');
  exec(`"${execPath}" get_date_transactions "${username}" "${from}" "${to}"`, (error, stdout, stderr) => {
    if (error) {
      console.error(`Exec error: ${error.message}`);
      return res.status(500).json({ error: error.message });
    }
    if (stderr) {
      console.error(`Stderr: ${stderr}`);
      return res.status(500).json({ error: stderr });
    }
    // console.log(`stdout: ${stdout}`); // Log stdout to ensure all transactions are fetched
    const transactions = stdout.split('\n').filter(line => line.trim() !== '');
    res.json({ transactions: transactions });
  });
});

app.get('/api/categorywisespend', (req,res)=>{
  const {username}=req.query;
  const execPath = path.resolve(__dirname, '../backend/finance_tracker');
  exec(`"${execPath}" get_piechart_values "${username}"`, (error, stdout, stderr) =>{
    if (error) {
      console.error(`Exec error: ${error.message}`);
      return res.status(500).json({ error: error.message });
    }
    if (stderr) {
      console.error(`Stderr: ${stderr}`);
      return res.status(500).json({ error: stderr });
    }
    // console.log(`stdout: ${stdout}`);
    
    const lines = stdout.split('\n').filter(line => line.trim() !== '');
    const categorySpend = {};
    lines.forEach(line => {
      const [category, amount] = line.split(' ');
      categorySpend[category] = parseFloat(amount);
    });

    res.json(categorySpend);
  })
})

app.post('/api/setbudget', (req, res) => {
  const { username, budget } = req.body;  // Ensure data is extracted from req.body
  
  console.log('Received request to set budget for:', username);
  // console.log('Budget data:', budget);

  if (!username || !budget) {
    return res.status(400).json({ error: 'Username and budget are required' });
  }

  const execPath = path.resolve(__dirname, '../backend/finance_tracker');
  let command = `"${execPath}" set_budget "${username}"`;

  // Add the categories and amounts to the command
  for (const [category, amount] of Object.entries(budget)) {
    command += ` "${category}" "${amount}"`;
  }

  // Log the command being executed
  // console.log('Command to execute:', command);

  exec(command, (error, stdout, stderr) => {
    if (error) {
      console.error(`Exec error: ${error.message}`);
      return res.status(500).json({ error: error.message });
    }
    if (stderr) {
      console.error(`Stderr: ${stderr}`);
      return res.status(500).json({ error: stderr });
    }

    // console.log(`stdout: ${stdout}`);
    res.json({ message: "Budget set successfully" });
  });
});

app.get('/api/getrecommendation', (req, res) => {
  const { username } = req.query;
  console.log("getting recommendations.......");
  console.log(`recommendation username: ${username}`);
  const execPath = path.resolve(__dirname, '../backend/finance_tracker');
  exec(`"${execPath}" fetch_recommendation "${username}"`, (error, stdout, stderr) => {
    if (error) {
      console.error(`Exec error: ${error.message} stdout: ${stdout}`);
      return res.status(500).json({ error: error.message });
    }
    if (stderr) {
      console.error(`Stderr: ${stderr}`);
      return res.status(500).json({ error: stderr });
    }
    const recommendations = stdout.split('\n').filter(line => line.trim() !== '');
    res.json({ recommendation: recommendations });
  });
});



app.listen(port, () => {
  console.log(`Server running at http://localhost:${port}`);
});
