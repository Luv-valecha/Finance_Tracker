const express = require('express');
const router = express.Router();
const { exec } = require('child_process');
const path = require('path');
const execPath = path.resolve(__dirname, '../../../backend/finance_tracker');

//                     This file handles the frontend api calls executing the backend and returning the result

// Handle user registration
router.post('/register', (req, res) => {
    const { username, password } = req.body;
    console.log(`Registering user: ${username}`);
    const execPath = path.resolve(__dirname, '../../backend/finance_tracker');
    
    exec(`"${execPath}" register "${username}" "${password}"`, (error, stdout, stderr) => {
        if (error) {
            console.error(`Exec error: ${error.message}`);
            return res.status(500).json({ error: error.message });
        } else if (stderr) {
            console.error(`Stderr: ${stderr}`);
            return res.status(500).json({ error: stderr });
        } else  {
            return res.json({ message: "User registered successfully", redirect: "/login" });
        }
    });
});


// Handle user login
router.post('/login', (req, res) => {
    const { username, password } = req.body;
    const execPath = path.resolve(__dirname, '../../backend/finance_tracker');
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
            console.log(`${username} logged in`);
            return res.json({ message: "Login successful", username: username, redirect: `/app?username=${username}` });
        } else {
            console.log("Unexpected output:", firstLine);
            return res.status(401).json({ message: firstLine });
        }
    });
});

// Handle logout
router.get('/logout', (req, res) => {
    console.log("Logging out user");
    req.session.destroy((err) => {
        if (err) {
            res.status(500).json({ error: "Could not log out" });
        } else {
            res.json({ message: "Logged out successfully" });
        }
    });
});

// Handle adding a new transaction
router.post('/add-transaction', (req, res) => {
    const { date, amount, category, description, username } = req.body;
    const execPath = path.resolve(__dirname, '../../backend/finance_tracker');
    exec(`"${execPath}" add_transaction "${username}" "${date}" ${amount} "${category}" "${description.replace(/"/g, '\\"')}"`, (error, stdout, stderr) => {
        if (error) {
            console.error(`Exec error: ${error.message}`);
            return res.status(500).json({ error: error.message });
        }
        if (stderr) {
            console.error(`Stderr: ${stderr}`);
            return res.status(500).json({ error: stderr });
        }
        console.log(`New Transaction added for user: ${username}`);
        res.json({ message: stdout.trim() });
    });
});

// Handle getting all transactions
router.get('/transactions', (req, res) => {
    const { username } = req.query;
    const execPath = path.resolve(__dirname, '../../backend/finance_tracker');
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
router.get('/cattransactions', (req, res) => {
    const { username, category } = req.query;
    const execPath = path.resolve(__dirname, '../../backend/finance_tracker');
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
router.get('/daterangetransactions', (req, res) => {
    const { username, from, to } = req.query;
    const execPath = path.resolve(__dirname, '../../backend/finance_tracker');
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

//making pie-chart
router.get('/categorywisespend', (req, res) => {
    const { username } = req.query;
    const execPath = path.resolve(__dirname, '../../backend/finance_tracker');
    exec(`"${execPath}" get_piechart_values "${username}"`, (error, stdout, stderr) => {
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
});

// Handle budget setting
router.post('/setbudget', (req, res) => {
    const { username, budget } = req.body;  // Ensure data is extracted from req.body

    console.log('Received request to set budget for:', username);
    // console.log('Budget data:', budget);

    if (!username || !budget) {
        return res.status(400).json({ error: 'Username and budget are required' });
    }

    const execPath = path.resolve(__dirname, '../../backend/finance_tracker');
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

// Handle recommendation generation
router.get('/getrecommendation', (req, res) => {
    const { username } = req.query;
    const execPath = path.resolve(__dirname, '../../backend/finance_tracker');
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

// Handle monthly statistics
router.get('/getstats', (req, res) => {
    const { username, month, year } = req.query;
    const execPath = path.resolve(__dirname, '../../backend/finance_tracker');
    exec(`"${execPath}" get_statistics "${username}" "${month}" "${year}"`, (error, stdout, stderr) => {
        if (error) {
            console.error(`Exec error: ${error.message}`);
            return res.status(500).json({ error: error.message });
        }
        if (stderr) {
            console.error(`Stderr: ${stderr}`);
            return res.status(500).json({ error: stderr });
        }
        const stats = {};
        const details = stdout.split('\n');
        details.forEach(deet => {
            currdeet = deet.split(" ");
            stats[currdeet[0]] = parseFloat(currdeet[1]);
        })
        res.json(stats);
    });
})

module.exports = router;