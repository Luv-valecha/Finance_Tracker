// Registration form submission
const registerForm = document.getElementById('registerForm');
if (registerForm) {
  registerForm.addEventListener('submit', (event) => {
    handleSubmit(event, '/api/register', (result) => {
      if (result.message === "User registered successfully") {
        alert(result.message);
      } else {
        showError(result.error);
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
        showError("Invalid username or password");
      }
    });
  });
}

//display the username on the main dashboard
const userHeader = document.getElementById("User_detail_heading");
const username = getUsername();
userHeader.innerHTML = `<p> <i class="fa-solid fa-user"></i> User: ${username}</p>`;

//main dashboard transactions "show more" button working
let showingall = false;
const showallbutton = document.getElementById('ShowFullList');
showallbutton.addEventListener('click', () => {
  showingall = !showingall;
  document.getElementById("MainPagePaginationControls").classList.toggle("hidden");
  // if (dateRangePaginationCall) loadDateRangeTransactions(document.getElementById('fromDate').value, document.getElementById('toDate').value);
  // else if (catWisePaginationCall) loadCategoryTransactions(document.getElementById('categoryinput').value);
  if (FilteredPaginationCall) loadFilterTransactions(document.getElementById('categoryinput').value, document.getElementById('typeinput').value, document.getElementById('fromDate').value, document.getElementById('toDate').value)
  else loadTransactions();
})

//pagination for all transactions page
const transactionPerPageButton = document.getElementById("transactionPerPage");
let transactionsPerPage = transactionPerPageButton.value;
transactionPerPageButton.addEventListener("change", () => {
  transactionsPerPage = transactionPerPageButton.value;
  loadTransactions();
})
let totalTransactions = 0;
let currentPage = 1;
let dateRangePaginationCall = false;
let catWisePaginationCall = false;
let FilteredPaginationCall = false;

// Automatically load transactions when the page loads
document.addEventListener('DOMContentLoaded', () => {
  loadTransactions();
  fetchCategorySpendData();
  get_recommendations();
  create_bar_graph();
});

// Add transaction form submission
const addTransactionForm = document.getElementById('addTransactionForm');
if (addTransactionForm) {
  addTransactionForm.addEventListener('submit', async (event) => {
    const anomaly = await detect_anomaly(event);
    if (anomaly["prediction"] === "Anomaly") {
      // Prompt user if they want to ignore the anomaly
      // const userDecision = confirm("This transaction seems like an anomaly and does not align with your past spending patterns, do you wish to proceed?");
      const { value: userDecision } = await Swal.fire({
        title: 'Anomaly Detected',
        text: "This transaction seems like an anomaly and does not align with your past spending patterns. Do you wish to proceed?",
        icon: 'warning',
        showCancelButton: true,
        confirmButtonText: 'Yes, Proceed',
        cancelButtonText: 'Cancel',
        reverseButtons: true
      });
      
      if (!userDecision) {
        // If user selects 'Cancel' (i.e., does not ignore the anomaly), stop the function
        event.preventDefault();  // This prevents form submission
        return; // Terminate the function
      }
    }
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
}

// Logout functionality
const logoutButton = document.getElementById('logoutButton');
if (logoutButton) {
  logoutButton.addEventListener('click', () => {
    fetch('/api/logout')
      .then(response => {
        if (response.status === 401) {
          window.location.href = '/login';
        }
        return response.json()
      })
      .then(result => {
        // alert(result.message);
        window.location.href = '/login';
      })
      .catch(error => {
        console.error('Error:', error);
        showError('An error occurred during logout. Please try again.');
      });
  });
}

// Handle filter transactions form submission
const filterTransactions = document.getElementById('filterTransactions');

if (filterTransactions) {
  filterTransactions.addEventListener('submit', (event) => {
    event.preventDefault();
    // console.log('Category form submitted'); // Log form submission
    const category = document.getElementById('categoryinput').value;
    const type = document.getElementById('typeinput').value;
    let fromDate = document.getElementById('fromDate').value || "";
    let toDate = document.getElementById('toDate').value || "";

    if (fromDate != "" && toDate == "") toDate = new Date().toISOString().split("T")[0];
    else if (toDate != "" && fromDate == "") {
      fromDate = new Date();
      fromDate.setFullYear(fromDate.getFullYear() - 500); // Go 500 years back
      fromDate = fromDate.toISOString().split("T")[0];
    }
    console.log(`Category: ${category} Type: ${type}, From: ${fromDate}, To: ${toDate}`); // Log category value
    loadFilterTransactions(category, type, fromDate, toDate);
  });
}

// // Handle type form submission
// const typeForm = document.getElementById('typeForm');
// if (typeForm) {
//   typeForm.addEventListener('submit', (event) => {
//     event.preventDefault();
//     // console.log('Category form submitted'); // Log form submission
//     const category = document.getElementById('typeinput').value;
//     // console.log(`Category: ${category}`); // Log category value
//     loadTypeTransactions(category);
//   });
// }

// // Handle date range form submission
// const dateRangeForm = document.getElementById('dateRangeForm');
// if (dateRangeForm) {
//   dateRangeForm.addEventListener('submit', (event) => {
//     event.preventDefault();
//     // console.log('Date range form submitted'); // Log form submission
//     const fromDate = document.getElementById('fromDate').value;
//     const toDate = document.getElementById('toDate').value;
//     // console.log(`From Date: ${fromDate}, To Date: ${toDate}`); // Log dates
//     loadDateRangeTransactions(fromDate, toDate);
//   });
// }

// Get buttons and form elements
const showTransactionFormButton = document.getElementById('showTransactionForm');
const showGetTransactionsFormButton = document.getElementById('showGetTransactionsForm');
const showSetBudgetFormButton = document.getElementById('showSetBudgetForm');

const transactionForm = document.getElementById('transactionForm');
const getTransactionsForm = document.getElementById('getTransactionsForm');
const budgetForm = document.getElementById('budgetForm');

// Show Add Transaction Form on button click and hide others
showTransactionFormButton.addEventListener('click', () => {
  document.getElementById("left-column").classList.remove("hidden");
  document.getElementById("right-column").classList.remove("hidden");
  if (!getTransactionsForm.classList.contains("hidden")) {
    catWisePaginationCall = false;
    dateRangePaginationCall = false;
    FilteredPaginationCall = false;
    loadTransactions();
  }
  currentPage = 1;
  aboutpage.classList.add("hidden");
  transactionForm.classList.toggle('hidden');
  getTransactionsForm.classList.add('hidden'); // Hide the other form
  budgetForm.classList.add('hidden');
  if (!allTransactionspage.classList.contains("hidden")) {
    allTransactionspage.classList.toggle("hidden");
    displayalltransactions = false;
    loadTransactions();
  }
  else if (!(statpage.classList.contains("hidden"))) statpage.classList.toggle("hidden");
});

// Show Get Transactions Form on button click and hide others
showGetTransactionsFormButton.addEventListener('click', () => {
  document.getElementById("left-column").classList.remove("hidden");
  document.getElementById("right-column").classList.remove("hidden");
  aboutpage.classList.add("hidden");
  if (!getTransactionsForm.classList.contains("hidden")) {
    catWisePaginationCall = false;
    dateRangePaginationCall = false;
    FilteredPaginationCall = false;
    loadTransactions();
  }
  currentPage = 1;
  getTransactionsForm.classList.toggle('hidden');
  if (!allTransactionspage.classList.contains("hidden")) {
    allTransactionspage.classList.toggle("hidden");
    displayalltransactions = false;
    loadTransactions();
  }
  else if (!(statpage.classList.contains("hidden"))) statpage.classList.toggle("hidden");
  transactionForm.classList.add('hidden'); // Hide the other form
  budgetForm.classList.add('hidden');
});

//Show set budget form on button click and hide others
showSetBudgetFormButton.addEventListener('click', () => {
  document.getElementById("left-column").classList.remove("hidden");
  document.getElementById("right-column").classList.remove("hidden");
  aboutpage.classList.add("hidden");
  budgetForm.classList.toggle('hidden');
  if (!getTransactionsForm.classList.contains("hidden")) {
    catWisePaginationCall = false;
    dateRangePaginationCall = false;
    FilteredPaginationCall = false;
    loadTransactions();
  }
  currentPage = 1;

  if (!allTransactionspage.classList.contains("hidden")) {
    allTransactionspage.classList.toggle("hidden");
    displayalltransactions = false;
    loadTransactions();
  }
  else if (!(statpage.classList.contains("hidden"))) statpage.classList.toggle("hidden");
  transactionForm.classList.add('hidden');
  getTransactionsForm.classList.add('hidden');
});

//set budget form submission
const setBudgetForm = document.getElementById('setBudgetForm');
if (setBudgetForm) {
  setBudgetForm.addEventListener('submit', (event) => {
    event.preventDefault();
    const username = getUsername(); // Assume this function returns the current user's username
    const categoryName = document.getElementById('budgetCategory').value;
    const budgetAmount = parseFloat(document.getElementById('budgetAmount').value);
    // Validation to ensure category and amount are valid
    if (!categoryName) {
      showError('Please select a category');
      return;
    }
    if (isNaN(budgetAmount) || budgetAmount <= 0) {
      showError('Please enter a valid budget amount');
      return;
    }
    // Construct budget object with category and amount
    const budget = {};
    budget[categoryName] = budgetAmount;
    // Call setBudget function to send the data to the server
    setBudget(username, budget);
  });
}

//Hide recommendations button working
var ShowingRecommendations = true;
var anyrecommendation = false;
const HideRecommendationButton = document.getElementById("ShowRecommendations");

HideRecommendationButton.addEventListener("click", () => {
  const recommendationListForm = document.getElementById("recommendationList");
  ShowingRecommendations = !ShowingRecommendations;
  recommendationListForm.classList.toggle("hidden", !ShowingRecommendations);
  HideRecommendationButton.innerHTML = ShowingRecommendations ? '<p> <i class="fa-regular fa-eye-slash"></i> Hide Warnings </p>'
    : '<p><i class="fa-regular fa-eye"></i> Show Warnings</p>';
});

//about page button working and page display
const aboutButton = document.getElementById("aboutButton");
const aboutpage = document.getElementById("page-about");
aboutButton.addEventListener(("click"), () => {
  aboutpage.classList.toggle("hidden");
  if (!getTransactionsForm.classList.contains("hidden")) {
    catWisePaginationCall = false;
    dateRangePaginationCall = false;
    FilteredPaginationCall = false;
    loadTransactions();
  }
  currentPage = 1;
  if (!allTransactionspage.classList.contains("hidden")) {
    allTransactionspage.classList.toggle("hidden");
    displayalltransactions = false;
    loadTransactions();
  }
  else if (!(statpage.classList.contains("hidden"))) statpage.classList.toggle("hidden");
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

//transaction history tab working
const showAllTransactionsButton = document.getElementById("showAllTransactions");
const allTransactionspage = document.getElementById("all-transactions");
var displayalltransactions = false;
showAllTransactionsButton.addEventListener(("click"), () => {
  if (!getTransactionsForm.classList.contains("hidden")) {
    catWisePaginationCall = false;
    dateRangePaginationCall = false;
    FilteredPaginationCall = false;
    loadTransactions();
  }
  currentPage = 1;
  if (!(aboutpage.classList.contains("hidden"))) aboutpage.classList.toggle("hidden");
  else if (!(statpage.classList.contains("hidden"))) statpage.classList.toggle("hidden");
  else {
    document.getElementById("left-column").classList.toggle("hidden");
    document.getElementById("right-column").classList.toggle("hidden");
  }
  allTransactionspage.classList.toggle("hidden");
  displayalltransactions = !displayalltransactions;
  loadTransactions();
})

//monthly stat page display and working
const statpage = document.getElementById("user-stats");
const statpageButton = document.getElementById("showStats");
statpageButton.addEventListener(("click"), () => {
  statpage.classList.toggle("hidden");
  if (!getTransactionsForm.classList.contains("hidden")) {
    catWisePaginationCall = false;
    dateRangePaginationCall = false;
    FilteredPaginationCall = false;
    loadTransactions();
  }
  currentPage = 1;

  if (!allTransactionspage.classList.contains("hidden")) {
    allTransactionspage.classList.toggle("hidden");
    displayalltransactions = false;
    loadTransactions();
  }
  else if (!aboutpage.classList.contains("hidden")) {
    aboutpage.classList.toggle("hidden");
    displayalltransactions = false;
    loadTransactions();
  }
  else {
    document.getElementById("left-column").classList.toggle("hidden");
    document.getElementById("right-column").classList.toggle("hidden");
  }
})

//monthly stats month input form
const statdateRangeForm = document.getElementById('statdateRangeForm');
if (statdateRangeForm) {
  statdateRangeForm.addEventListener('submit', (event) => {
    event.preventDefault();
    // console.log('Date range form submitted'); // Log form submission
    const month = document.getElementById('monthinput').value;
    const now = new Date();
    const year = now.getFullYear();
    loadStatistics(month, year);
  });
}
