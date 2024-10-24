function handleSubmit(event, url, successCallback) {
  event.preventDefault();
  const formData = new FormData(event.target);
  if (url !== '/api/register' && url !== '/api/login') {
    const username = getUsername();
    console.log(`Appending username to form data: ${username}`);  // Log the username being appended
    formData.append('username', username);
  }
  const data = Object.fromEntries(formData.entries());
  fetch(url, {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json',
    },
    body: JSON.stringify(data),
  })
    .then(response => response.json())
    .then(result => {
      if (result.redirect) {
        // alert(result.message || 'Operation completed successfully');
        window.location.href = result.redirect;
      } else {
        // alert(result.message || 'Operation completed successfully');
        if (successCallback) {
          successCallback(result);
        }
      }
    })
    .catch(error => {
      console.error('Error:', error);
      alert('An error occurred. Please try again. Details: ' + error.message);
    });
}

// function getUsername() {
//     const name = "username=";
//     const decodedCookie = decodeURIComponent(document.cookie);
//     console.log(`Decoded cookies: ${decodedCookie}`); // Log all decoded cookies
//     const ca = decodedCookie.split(';');
//     for (let i = 0; i < ca.length; i++) {
//       let c = ca[i].trim();
//       console.log(`Processing cookie: ${c}`);  // Log each cookie being processed
//       if (c.indexOf(name) === 0) {
//         const username = c.substring(name.length, c.length);
//         console.log(`Retrieved username from cookie: ${username}`);  // Log the retrieved username
//         return username;
//       }
//     }
//     console.log("Username not found in cookies");  // Log if username is not found
//     return "";  // Return an empty string if username is not found
//   }


// Registration form submission
const registerForm = document.getElementById('registerForm');
if (registerForm) {
  registerForm.addEventListener('submit', (event) => {
    handleSubmit(event, '/api/register', (result) => {
      if (result.message === "User registered successfully") {
        alert(result.message);
      } else {
        alert(result.message || "Error registering");
      }
    });
  });
}

// Login form submission
const loginForm = document.getElementById('loginForm');
if (loginForm) {
  loginForm.addEventListener('submit', (event) => {
    handleSubmit(event, '/api/login', (result) => {
      if (result.message === "Login successful") {
        sessionStorage.setItem('username', result.username);  // Store username in session storage
        window.location.href = result.redirect;
      } else {
        alert(result.message || "Error Logging in");
      }
    });
  });
}

// Function to extract username from URL
function getUsername() {
  const urlParams = new URLSearchParams(window.location.search);
  const username = urlParams.get('username');
  console.log(`Retrieved username from URL: ${username}`);
  return username || "";  // Return empty string if username is not found
}

const userHeader = document.getElementById("User_detail_heading");
const username = getUsername();
userHeader.innerHTML = `<p> <i class="fa-solid fa-user"></i> User: ${username}</p>`;

//   // Use this function instead of getUsername if extracting from URL
//   const username = getUsernameFromURL();


// // Add transaction form submission
// const addTransactionForm = document.getElementById('addTransactionForm');
// if (addTransactionForm) {
//   addTransactionForm.addEventListener('submit', (event) => {
//     handleSubmit(event, '/api/add-transaction', () => {
//       loadTransactions();
//     });
//   });
// }
var showingall = false;
const showallbutton = document.getElementById('ShowFullList');
showallbutton.addEventListener('click', () => {
  showingall = !showingall;
  loadTransactions();
})

function loadTransactions() {
  const username = getUsername();
  fetch(`/api/transactions?username=${username}`)
    .then(response => response.json())
    .then(data => {
      // console.log(data.transactions);  // Log fetched transactions for debugging
      const transactionList = document.getElementById('transactions');
      const alltransactionList = document.getElementById('alltransactions');
      transactionList.innerHTML = ''; // Clear previous transactions
      alltransactionList.innerHTML = '';
      if (data.transactions.length === 0) {
        transactionList.innerHTML = '<li>No Transactions Available</li>';
      }
      else if (displayalltransactions) {
        data.transactions.forEach(transaction => {
          const transactionparams = transaction.split(',');
          const li = document.createElement('li');
          li.innerHTML = transactionparams.join('<br>');
          alltransactionList.appendChild(li);
        });
      }
      else {
        if (!showingall) {
          const transactionsToDisplay = data.transactions.length > 5
            ? data.transactions.slice(-6) // Get the last 5 transactions
            : data.transactions; // If 5 or fewer, display all

          // Render the transactions
          transactionsToDisplay.forEach(transaction => {
            const transactionparams = transaction.split(',');
            const li = document.createElement('li');
            li.innerHTML = transactionparams.join('<br>');
            transactionList.appendChild(li);
          });
        }
        else {
          const transactionsToDisplay = data.transactions.length > 5
            ? data.transactions.slice(-12) // Get the last 5 transactions
            : data.transactions; // If 5 or fewer, display all

          // Render the transactions
          transactionsToDisplay.forEach(transaction => {
            const transactionparams = transaction.split(',');
            const li = document.createElement('li');
            li.innerHTML = transactionparams.join('<br>');  // Adjust based on how your transaction data is structured
            transactionList.appendChild(li);
          });
        }
        showallbutton.innerHTML = showingall ? '<p><i class="fa-regular fa-eye-slash"></i> Show Less</p>'
          : '<p><i class="fa-regular fa-eye"></i> Show More</p>';
      }
    })
    .catch(error => {
      console.error('Error fetching transactions:', error);
    });
}

// function loadAllTransactions() {
//   const username = getUsername();
//   fetch(`/api/transactions?username=${username}`)
//     .then(response => response.json())
//     .then(data => {
//       console.log(data.transactions);  // Log fetched transactions for debugging
//       const transactionList = document.getElementById('transactions');
//       transactionList.innerHTML = ''; // Clear previous transactions
//       if (data.transactions.length === 0) {
//         transactionList.innerHTML = '<li>No Transactions Available</li>';
//       } else {
//         data.transactions.forEach(transaction => {
//           const li = document.createElement('li');
//           li.textContent = transaction;
//           transactionList.appendChild(li);
//         });
//       }
//     })
//     .catch(error => {
//       console.error('Error fetching transactions:', error);
//     });
// }

// Automatically load transactions when the page loads
document.addEventListener('DOMContentLoaded', () => {
  loadTransactions();
  fetchCategorySpendData();
  get_recommendations();
});

// Fetch category spend data and create the pie chart
function fetchCategorySpendData() {
  const username = getUsername();
  fetch(`/api/categorywisespend?username=${username}`)
    .then(response => response.json())
    .then(data => {
      createPieChart(data);
    })
    .catch(error => {
      console.error('Error fetching category spend data:', error);
    });
}

// Create a pie chart using the fetched data
function createPieChart(data) {
  console.log(data.transactions);
  const ctx = document.getElementById('categorySpendChart').getContext('2d');

  const labels = Object.keys(data);
  const values = Object.values(data);

  const chartData = {
    labels: labels,
    datasets: [{
      data: values,
      backgroundColor: [
        '#FF6384',
        '#36A2EB',
        '#FFCE56',
        '#FF5733',
        '#C70039',
        '#900C3F',
        '#581845'
      ],
    }]
  };

  new Chart(ctx, {
    type: 'pie',
    data: chartData,
    options: {
      responsive: false,
      plugins: {
        legend: {
          labels: {
            color: 'aliceblue' // Change legend text color to aliceblue
          }
        }
      }
    }
  });
}

// Add transaction form submission
const addTransactionForm = document.getElementById('addTransactionForm');
if (addTransactionForm) {
  addTransactionForm.addEventListener('submit', (event) => {
    handleSubmit(event, '/api/add-transaction', () => {
      loadTransactions();
      get_recommendations();
      location.reload();
    });
  });
}

// Load transactions when the main app page loads
if (window.location.pathname === '/app') {
  loadTransactions();
  // get_recommendations();
  // fetchCategorySpendData();
}

// Logout functionality
const logoutButton = document.getElementById('logoutButton');
if (logoutButton) {
  logoutButton.addEventListener('click', () => {
    fetch('/api/logout')
      .then(response => response.json())
      .then(result => {
        alert(result.message);
        window.location.href = '/login';
      })
      .catch(error => {
        console.error('Error:', error);
        alert('An error occurred during logout. Please try again.');
      });
  });
}

// Handle category form submission
const categoryForm = document.getElementById('categoryForm');
if (categoryForm) {
  categoryForm.addEventListener('submit', (event) => {
    event.preventDefault();
    console.log('Category form submitted'); // Log form submission
    const category = document.getElementById('categoryinput').value;
    console.log(`Category: ${category}`); // Log category value
    loadCategoryTransactions(category);
  });
}

// Function to fetch category-wise transactions
function loadCategoryTransactions(category) {
  const username = getUsername();
  console.log(`Fetching transactions for category: ${category}`); // Log category fetch
  fetch(`/api/cattransactions?username=${username}&category=${category}`)
    .then(response => response.json())
    .then(data => {
      console.log(data.transactions);  // Log fetched transactions for debugging
      const transactionList = document.getElementById('transactions');
      transactionList.innerHTML = ''; // Clear previous transactions
      if (data.transactions.length === 0) {
        transactionList.innerHTML = '<li>No Transactions Available</li>';
      } else {
        const transactionsToDisplay = data.transactions.length > 5
          ? data.transactions.slice(-5) // Get the last 5 transactions
          : data.transactions; // If 5 or fewer, display all

        // Render the transactions
        transactionsToDisplay.forEach(transaction => {
          const li = document.createElement('li');
          li.textContent = transaction;  // Adjust based on how your transaction data is structured
          transactionList.appendChild(li);
        });
      }
    })
    .catch(error => {
      console.error('Error fetching transactions:', error);
    });
}


// Handle date range form submission
const dateRangeForm = document.getElementById('dateRangeForm');
if (dateRangeForm) {
  dateRangeForm.addEventListener('submit', (event) => {
    event.preventDefault();
    console.log('Date range form submitted'); // Log form submission
    const fromDate = document.getElementById('fromDate').value;
    const toDate = document.getElementById('toDate').value;
    console.log(`From Date: ${fromDate}, To Date: ${toDate}`); // Log dates
    loadDateRangeTransactions(fromDate, toDate);
  });
}

// Function to fetch transactions within a date range
function loadDateRangeTransactions(fromDate, toDate) {
  const username = getUsername();
  console.log(`Fetching transactions from ${fromDate} to ${toDate}`); // Log date fetch
  fetch(`/api/daterangetransactions?username=${username}&from=${fromDate}&to=${toDate}`)
    .then(response => response.json())
    .then(data => {
      console.log(data.transactions);  // Log fetched transactions for debugging
      const transactionList = document.getElementById('transactions');
      transactionList.innerHTML = ''; // Clear previous transactions
      if (data.transactions.length === 0) {
        transactionList.innerHTML = '<li>No Transactions Available</li>';
      } else {
        data.transactions.forEach(transaction => {
          const li = document.createElement('li');
          li.textContent = transaction;
          transactionList.appendChild(li);
        });
      }
    })
    .catch(error => {
      console.error('Error fetching transactions:', error);
    });
}

const getalltrans = document.getElementById('getalltrans');
document.addEventListener('click', () => {
  loadTransactions();
})

// Get buttons and form elements
const showTransactionFormButton = document.getElementById('showTransactionForm');
const showGetTransactionsFormButton = document.getElementById('showGetTransactionsForm');
const showSetBudgetFormButton = document.getElementById('showSetBudgetForm');

const transactionForm = document.getElementById('transactionForm');
const getTransactionsForm = document.getElementById('getTransactionsForm');
const budgetForm = document.getElementById('budgetForm');

// Show Add Transaction Form on button click
showTransactionFormButton.addEventListener('click', () => {
  document.getElementById("left-column").classList.remove("hidden");
  document.getElementById("right-column").classList.remove("hidden");
  aboutpage.classList.add("hidden");
  transactionForm.classList.toggle('hidden');
  getTransactionsForm.classList.add('hidden'); // Hide the other form
  budgetForm.classList.add('hidden');
  if (!allTransactionspage.classList.contains("hidden")) {
    allTransactionspage.classList.toggle("hidden");
    displayalltransactions = false;
    loadTransactions();
  }
});

// Show Get Transactions Form on button click
showGetTransactionsFormButton.addEventListener('click', () => {
  document.getElementById("left-column").classList.remove("hidden");
  document.getElementById("right-column").classList.remove("hidden");
  aboutpage.classList.add("hidden");
  getTransactionsForm.classList.toggle('hidden');
  if (!allTransactionspage.classList.contains("hidden")) {
    allTransactionspage.classList.toggle("hidden");
    displayalltransactions = false;
    loadTransactions();
  }
  transactionForm.classList.add('hidden'); // Hide the other form
  budgetForm.classList.add('hidden');
});

showSetBudgetFormButton.addEventListener('click', () => {
  document.getElementById("left-column").classList.remove("hidden");
  document.getElementById("right-column").classList.remove("hidden");
  aboutpage.classList.add("hidden");
  budgetForm.classList.toggle('hidden');
  if (!allTransactionspage.classList.contains("hidden")) {
    allTransactionspage.classList.toggle("hidden");
    displayalltransactions = false;
    loadTransactions();
  }
  transactionForm.classList.add('hidden');
  getTransactionsForm.classList.add('hidden');
});

const setBudgetForm = document.getElementById('setBudgetForm');
if (setBudgetForm) {
  setBudgetForm.addEventListener('submit', (event) => {
    event.preventDefault();
    const username = getUsername(); // Assume this function returns the current user's username
    const categoryName = document.getElementById('budgetCategory').value;
    const budgetAmount = parseFloat(document.getElementById('budgetAmount').value);
    // Validation to ensure category and amount are valid
    if (!categoryName) {
      alert('Please select a category');
      return;
    }
    if (isNaN(budgetAmount) || budgetAmount <= 0) {
      alert('Please enter a valid budget amount');
      return;
    }
    // Construct budget object with category and amount
    const budget = {};
    budget[categoryName] = budgetAmount;
    // Call setBudget function to send the data to the server
    setBudget(username, budget);
  });
}


// Set budget function
function setBudget(username, budget) {
  console.log('Sending username:', username);
  console.log('Sending budget:', budget);

  // Send the data as JSON in the request body
  fetch(`/api/setbudget`, {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json',
    },
    body: JSON.stringify({ username, budget }),  // Correctly structure the JSON body
  })
    .then(response => {
      if (!response.ok) {
        throw new Error('Network response was not OK');
      }
      return response.json();
    })
    .then(data => {
      console.log('Response from server:', data);
      if (data.message) {
        alert(data.message);
      } else {
        alert('Unexpected response format');
      }
    })
    .catch(error => {
      console.error('Error setting budget:', error);
      // alert('Failed to set budget, please try again');
    });
  location.reload();
}

function get_recommendations() {
  const recommendationListForm = document.getElementById("recommendationList");
  const recommendationList = document.getElementById("recommendations");
  const username = getUsername();

  if (ShowingRecommendations) recommendationListForm.classList.remove("hidden");

  fetch(`/api/getrecommendation?username=${username}`)
    .then(response => {
      if (!response.ok) {
        throw new Error('Network response was not OK');
      }
      return response.json();
    })
    .then(data => {
      const dataarray = data.recommendation || [];
      recommendationList.innerHTML = '';
      if (dataarray.length != 0) {
        anyrecommendation = true;
        data.recommendation.forEach(recommendation => {
          // Split the recommendation string to extract the amount
          const parts = recommendation.split(" by "); // Split by " by "
          const amount = parseFloat(parts[1]); // Get the amount part    
          // Format the amount to two decimal places
          const formattedAmount = amount.toFixed(2);
          // Create a new recommendation string with the formatted amount
          const updatedRecommendation = `${parts[0]} by ${formattedAmount}`;
          // Create a list item for the updated recommendation
          const li = document.createElement('li');
          li.textContent = updatedRecommendation; // Use the updated recommendation text
          recommendationList.appendChild(li);
        });
      }
      else anyrecommendation = false;
      console.log(dataarray.length);
      if (anyrecommendation) HideRecommendationButton.classList.remove("hidden");
      else HideRecommendationButton.classList.add("hidden")
    })
    .catch(error => {
      console.error('Error fetching recommendations:', error);
    });
}

var ShowingRecommendations = true;
var anyrecommendation = false;
const HideRecommendationButton = document.getElementById("ShowRecommendations");

HideRecommendationButton.addEventListener("click", () => {
  const recommendationListForm = document.getElementById("recommendationList");
  ShowingRecommendations = !ShowingRecommendations;
  recommendationListForm.classList.toggle("hidden", !ShowingRecommendations);
  HideRecommendationButton.innerHTML = ShowingRecommendations ? '<p> <i class="fa-regular fa-eye-slash"></i> Hide Recommendations </p>'
    : '<p><i class="fa-regular fa-eye"></i> Show Recommendations</p>';
});

const aboutButton = document.getElementById("aboutButton");
const aboutpage = document.getElementById("page-about");
aboutButton.addEventListener(("click"), () => {
  aboutpage.classList.toggle("hidden");
  if (!allTransactionspage.classList.contains("hidden")) {
    allTransactionspage.classList.toggle("hidden");
    displayalltransactions = false;
    loadTransactions();
  }
  else {
    document.getElementById("left-column").classList.toggle("hidden");
    document.getElementById("right-column").classList.toggle("hidden");
  }
})

//to display the calendar to pick date if we click anywhere on the input block instead of just the right corner
const dateinput1 = document.getElementById("date");
dateinput1.addEventListener('click', function () {
  dateinput1.showPicker(); // Show the date picker
});
const dateinput2 = document.getElementById("fromDate");
dateinput2.addEventListener('click', function () {
  dateinput2.showPicker(); // Show the date picker
});
const dateinput3 = document.getElementById("toDate");
dateinput3.addEventListener('click', function () {
  dateinput3.showPicker(); // Show the date picker
});

const showAllTransactionsButton = document.getElementById("showAllTransactions");
const allTransactionspage = document.getElementById("all-transactions");
var displayalltransactions = false;
showAllTransactionsButton.addEventListener(("click"), () => {
  if (!(aboutpage.classList.contains("hidden"))) aboutpage.classList.toggle("hidden");
  else {
    document.getElementById("left-column").classList.toggle("hidden");
    document.getElementById("right-column").classList.toggle("hidden");
  }
  allTransactionspage.classList.toggle("hidden");
  displayalltransactions = !displayalltransactions;
  loadTransactions();
})