//function to handle form submission
function handleSubmit(event, url, successCallback) {
    event.preventDefault();
    const formData = new FormData(event.target);
    if (url !== '/api/register' && url !== '/api/login') {
      const username = getUsername();
      // console.log(`Appending username to form data: ${username}`);  // Log the username being appended
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
      .then(response => {
        if (response.status === 401) {
          window.location.href = '/login';
        }
        return response.json()
      })
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
        showError('An error occurred. Please try again. Details: ' + error.message);
      });
  }
  
  //function for error display
  function showError(message) {
    Swal.fire({
      icon: 'error',
      title: 'Error...',
      text: message,
      width: 400,
      height: 300,
      padding: '1em',
      confirmButtonText: 'Ok',
      timer: 1800,
      timerProgressBar: true,
    });
  }

// Function to extract username from URL
function getUsername() {
    const urlParams = new URLSearchParams(window.location.search);
    const username = urlParams.get('username');
    return username || "";  // Return empty string if username is not found
  }

//enable or disable pagination buttons
function updatePaginationControls(totalTransactions) {
    const totalPages = Math.ceil(totalTransactions / transactionsPerPage);
    document.getElementById("pageInfo").textContent = `Page ${currentPage} of ${totalPages}`;
    document.getElementById("MainPageInfo").textContent = `Page ${currentPage} of ${totalPages}`;
  
    // Enable or disable pagination buttons based on the current page
    document.getElementById("MainNextPage").disabled = currentPage === totalPages;
    document.getElementById("MainPrevPage").disabled = currentPage === 1;
    document.getElementById("prevPage").disabled = currentPage === 1;
    document.getElementById("nextPage").disabled = currentPage === totalPages;
  }
  
  //change page function
  function changePage(direction) {
    const totalPages = Math.ceil(totalTransactions / transactionsPerPage);
    // Update current page within range
    currentPage += direction;
    if (currentPage < 1) currentPage = 1;
    if (currentPage > totalPages) currentPage = totalPages;
  
    // Re-render transactions for the new page
    if (dateRangePaginationCall) loadDateRangeTransactions(document.getElementById('fromDate').value, document.getElementById('toDate').value);
    else if (catWisePaginationCall) loadCategoryTransactions(document.getElementById('categoryinput').value);
    else loadTransactions();
  }
  
  //function to load transactions to display
  function loadTransactions() {
    dateRangePaginationCall = false;
    catWisePaginationCall = false;
    const username = getUsername();
    fetch(`/api/transactions?username=${username}`)
      .then(response => {
        if (response.status === 401) {
          window.location.href = '/login';
        }
        return response.json()
      })
      .then(data => {
        console.log(data.transactions);  // Log fetched transactions for debugging
        const transactionList = document.getElementById('transactions');
        const alltransactionList = document.getElementById('alltransactions');
        transactionList.innerHTML = ''; // Clear previous transactions
        alltransactionList.innerHTML = '';
        totalTransactions = data.transactions.length;
        if (data.transactions.length === 0) {
          transactionList.innerHTML = '<li>No Transactions Available</li>';
        }
        else if (displayalltransactions) {
          const totalPages = Math.ceil(totalTransactions / transactionsPerPage);
          if (currentPage < 1) currentPage = 1;
          if (currentPage > totalPages) currentPage = totalPages;
          const startIndex = (parseInt(currentPage) - 1) * parseInt(transactionsPerPage);
          const endIndex = startIndex + parseInt(transactionsPerPage);
          const paginatedTransactions = data.transactions.slice(startIndex, endIndex);
          paginatedTransactions.forEach(transaction => {
            const transactionparams = transaction.split(',');
            const li = document.createElement('li');
            li.innerHTML = transactionparams.join('<br>');
            alltransactionList.appendChild(li);
          });
          updatePaginationControls(data.transactions.length);
        }
        else {
          if (!showingall) {
            const transactionsToDisplay = data.transactions.length > 5
              ? data.transactions.slice(-6) // Get the last 6 transactions
              : data.transactions; // If 6 or fewer, display all
  
            // Render the transactions
            transactionsToDisplay.forEach(transaction => {
              const transactionparams = transaction.split(',');
              const li = document.createElement('li');
              li.innerHTML = transactionparams.join('<br>');
              transactionList.appendChild(li);
            });
          }
          else {
            const totalPages = Math.ceil(totalTransactions / transactionsPerPage);
            if (currentPage < 1) currentPage = 1;
            if (currentPage > totalPages) currentPage = totalPages;
            const startIndex = (parseInt(currentPage) - 1) * parseInt(transactionsPerPage);
            const endIndex = startIndex + parseInt(transactionsPerPage);
            const paginatedTransactions = data.transactions.slice(startIndex, endIndex);
            paginatedTransactions.forEach(transaction => {
              const transactionparams = transaction.split(',');
              const li = document.createElement('li');
              li.innerHTML = transactionparams.join('<br>');
              transactionList.appendChild(li);
            });
            updatePaginationControls(data.transactions.length);
          }
          showallbutton.innerHTML = showingall ? '<p><i class="fa-regular fa-eye-slash"></i> Show Less</p>'
            : '<p><i class="fa-regular fa-eye"></i> Show More</p>';
        }
      })
      .catch(error => {
        console.error('Error fetching transactions:', error);
      });
  }

// Fetch category spend data and create the pie chart
function fetchCategorySpendData() {
    const username = getUsername();
    fetch(`/api/categorywisespend?username=${username}`)
      .then(response => {
        if (response.status === 401) {
          window.location.href = '/login';
        }
        return response.json()
      })
      .then(data => {
        createPieChart(data);
      })
      .catch(error => {
        console.error('Error fetching category spend data:', error);
      });
  }
  
  // Create a pie chart using the fetched data
  function createPieChart(data) {
    // console.log(data.transactions);
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

// Function to fetch category-wise transactions
function loadCategoryTransactions(category) {
    const username = getUsername();
    dateRangePaginationCall = false;
    catWisePaginationCall = true;
    // console.log(`Fetching transactions for category: ${category}`); // Log category fetch
    fetch(`/api/cattransactions?username=${username}&category=${category}`)
      .then(response => {
        if (response.status === 401) {
          window.location.href = '/login';
        }
        return response.json()
      })
      .then(data => {
        // console.log(data.transactions);  // Log fetched transactions for debugging
        const transactionList = document.getElementById('transactions');
        transactionList.innerHTML = ''; // Clear previous transactions
        if (data.transactions.length === 0) {
          transactionList.innerHTML = '<li>No Transactions Available</li>';
        } else {
          let transactionsToDisplay = data.transactions;
          if (!showingall) {
            transactionsToDisplay = data.transactions.length > 6
              ? data.transactions.slice(-6) // Get the last 6 transactions
              : data.transactions; // If 6 or fewer, display all
          }
          // Render the transactions
          if (!showingall) {
            transactionsToDisplay.forEach(transaction => {
              const transactionparams = transaction.split(',');
              const li = document.createElement('li');
              li.innerHTML = transactionparams.join('<br>');
              transactionList.appendChild(li);
            });
          }
          else {
            const totalPages = Math.ceil(transactionsToDisplay.length / transactionsPerPage);
            if (currentPage < 1) currentPage = 1;
            if (currentPage > totalPages) currentPage = totalPages;
            const startIndex = (currentPage - 1) * transactionsPerPage; //transactionspp=16 fixed
            const endIndex = startIndex + transactionsPerPage;
            const paginatedTransactions = transactionsToDisplay.slice(startIndex, endIndex);
            paginatedTransactions.forEach(transaction => {
              const transactionparams = transaction.split(',');
              const li = document.createElement('li');
              li.innerHTML = transactionparams.join('<br>');
              transactionList.appendChild(li);
            });
            updatePaginationControls(transactionsToDisplay.length);
          }
        }
      })
      .catch(error => {
        console.error('Error fetching transactions:', error);
      });
  }
  

// Function to fetch transactions within a date range
function loadDateRangeTransactions(fromDate, toDate) {
    const username = getUsername();
    dateRangePaginationCall = true;
    catWisePaginationCall = false;
    // console.log(`Fetching transactions from ${fromDate} to ${toDate}`); // Log date fetch
    fetch(`/api/daterangetransactions?username=${username}&from=${fromDate}&to=${toDate}`)
      .then(response => {
        if (response.status === 401) {
          window.location.href = '/login';
        }
        return response.json()
      })
      .then(data => {
        // console.log(data.transactions);  // Log fetched transactions for debugging
        const transactionList = document.getElementById('transactions');
        transactionList.innerHTML = ''; // Clear previous transactions
        if (data.transactions.length === 0) {
          transactionList.innerHTML = '<li>No Transactions Available</li>';
        } else {
          let transactionsToDisplay = data.transactions;
          if (!showingall) {
            transactionsToDisplay = data.transactions.length > 6
              ? data.transactions.slice(0, 6) // Get the first 6 transactions
              : data.transactions; // If 6 or fewer, display all
          }
          // Render the transactions
          if (!showingall) {
            transactionsToDisplay.forEach(transaction => {
              const transactionparams = transaction.split(',');
              const li = document.createElement('li');
              li.innerHTML = transactionparams.join('<br>');
              transactionList.appendChild(li);
            });
          }
          else {
            const totalPages = Math.ceil(transactionsToDisplay.length / transactionsPerPage);
            if (currentPage < 1) currentPage = 1;
            if (currentPage > totalPages) currentPage = totalPages;
            const startIndex = (currentPage - 1) * transactionsPerPage; //transactionspp=16 fixed
            const endIndex = startIndex + transactionsPerPage;
            const paginatedTransactions = transactionsToDisplay.slice(startIndex, endIndex);
            paginatedTransactions.forEach(transaction => {
              const transactionparams = transaction.split(',');
              const li = document.createElement('li');
              li.innerHTML = transactionparams.join('<br>');
              transactionList.appendChild(li);
            });
            updatePaginationControls(transactionsToDisplay.length);
          }
        }
      })
      .catch(error => {
        console.error('Error fetching transactions:', error);
      });
  }


// Set budget function api call
function setBudget(username, budget) {
    // console.log('Sending username:', username);
    // console.log('Sending budget:', budget);
  
    // Send the data as JSON in the request body
    fetch(`/api/setbudget`, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({ username, budget }),  // Correctly structure the JSON body
    })
      .then(response => {
        if (response.status === 401) {
          window.location.href = '/login';
        }
        return response.json()
      })
      .then(data => {
        // console.log('Response from server:', data);
        if (data.message) {
          showError(data.message);
        } else {
          showError('Unexpected response format');
        }
      })
      .catch(error => {
        console.error('Error setting budget:', error);
        // alert('Failed to set budget, please try again');
      });
    location.reload();
  }
  
  //api call to get recommendations
  function get_recommendations() {
    const recommendationListForm = document.getElementById("recommendationList");
    const recommendationList = document.getElementById("recommendations");
    const username = getUsername();
  
    if (ShowingRecommendations) recommendationListForm.classList.remove("hidden");
  
    fetch(`/api/getrecommendation?username=${username}`)
      .then(response => {
        if (response.status === 401) {
          window.location.href = '/login';
        }
        return response.json()
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
        // console.log(dataarray.length);
        if (anyrecommendation) HideRecommendationButton.classList.remove("hidden");
        else HideRecommendationButton.classList.add("hidden")
      })
      .catch(error => {
        console.error('Error fetching recommendations:', error);
      });
  }

//generating monthly bar graph on the main dashboard
async function getmonthlygraphvalues(fromDate, toDate) {
    //getting transactions
    const username = getUsername();
    let totalspend = 0;
    // console.log(`Fetching transactions from ${fromDate} to ${toDate}`); // Log date fetch
    try {
      const response = await fetch(`/api/daterangetransactions?username=${username}&from=${fromDate}&to=${toDate}`);
      if (response.status === 401) {
        window.location.href = '/login';
      }
      const data = await response.json();
  
      data.transactions.forEach(transaction => {
        if (transaction.trim() === "No Transactions Available") return;
        const amountMatch = transaction.match(/Amount:\s([\d\.]+)\sINR/);
  
        if (amountMatch) {
          const amount = parseFloat(amountMatch[1]); // Convert matched string to a float
          totalspend += amount; // Accumulate the total spend
        } else {
          console.warn("Pattern not matched in transaction:", transaction);
        }
      });
    } catch (error) {
      console.error('Error fetching transactions:', error);
    }
    return totalspend;
  }
  
  //function to get total days in a month
  function daysInMonth(month, year) {
    return new Date(year, month, 0).getDate(); // Correctly returns the last day of the month
  }
  
  //function to get the name of the month from its number
  function getMonthName(monthNumber) {
    const monthNames = [
      "January", "February", "March", "April", "May", "June",
      "July", "August", "September", "October", "November", "December"
    ];
    if (monthNumber < 1 || monthNumber > 12) {
      return "Invalid month number";
    }
    return monthNames[monthNumber - 1];
  }
  
  //function to create the main dashboard bar graph
  async function create_bar_graph() {
    const now = new Date();
  
    // Current date and time
    const year = now.getFullYear();
    const month = now.getMonth() + 1; // +1 because months are zero-indexed
    const bargraphvalues = [];
  
    for (let i = 0; i < 5; i++) {
      const currentMonth = month - i;
      let currentYear = year;
  
      // Adjust the year and month if the month goes below 1
      if (currentMonth <= 0) {
        currentYear -= 1; // Go to the previous year
        currentMonth += 12; // Wrap around to December
      }
  
      const fromDate = `${currentYear}-${String(currentMonth).padStart(2, '0')}-01`; // Ensure month is two digits
      const days = daysInMonth(currentMonth, currentYear); // Pass the correct month and year
      const toDate = `${currentYear}-${String(currentMonth).padStart(2, '0')}-${days}`; // Ensure month is two digits
  
      let totalspend = await getmonthlygraphvalues(fromDate, toDate); // Await the async function
      bargraphvalues.push({ month: `${getMonthName(currentMonth)}`, spend: totalspend });
    }
    bargraphvalues.reverse();
    const months = bargraphvalues.map(data => data.month);
    const amounts = bargraphvalues.map(data => data.spend);
  
    // Creating graph
    const ctx = document.getElementById("averageBarGraph").getContext("2d");
    new Chart(ctx, {
      type: 'bar',
      data: {
        labels: months,
        datasets: [{
          label: 'Monthly Expenditure',
          data: amounts,
          backgroundColor: 'rgba(185, 28, 28,0.9)',
          borderColor: 'rgba(255, 223, 0,0.7)',
          borderWidth: 2,
          hoverBackgroundColor: 'red', // Hover color
          hoverBorderColor: 'gold'
        }]
      },
      options: {
        responsive: true,
        scales: {
          y: {
            beginAtZero: true,
            title: {
              display: true,
              text: 'Amount (INR)',
              font: {
                size: 14,
              },
              color: 'aliceblue'
            },
            grid: {
              color: 'rgba(255,255,255,0)',
              lineWidth: 1
            },
            ticks: {
              color: 'aliceblue',
              font: {
                size: 12
              }
            }
          },
          x: {
            title: {
              display: true,
              text: 'Month',
              font: {
                size: 14,
              },
              color: 'aliceblue'
            },
            grid: {
              color: 'rgba(255,255,255,0)',
              lineWidth: 1
            },
            ticks: {
              color: 'aliceblue',
              font: {
                size: 12
              }
            }
          },
        },
        plugins: {
          legend: {
            labels: {
              color: 'aliceblue',
              font: {
                size: 16
              }
            }
          }
        }
      }
    });
  }

//getting the month stat as requested and generating the bar graph
let statBarGraph;
function loadStatistics(month, year) {
  const username = getUsername();
  // console.log("Loadin statistics....");
  fetch(`/api/getstats?username=${username}&month=${month}&year=${year}`)
    .then(response => {
      if (response.status === 401) {
        window.location.href = '/login';
      }
      return response.json()
    })
    .then(data => {
      const statList = document.getElementById("stat-list");
      // Clear previous list items
      statList.innerHTML = "";
      const catvalues = [];
      let anytransaction = false;
      // Iterate over each key-value pair in data
      Object.entries(data).forEach(([key, value]) => {
        // Create a new list item
        const listItem = document.createElement("li");
        // console.log(key);
        if (key == "Total_Spend" || key == "Month_Average" || key == "Month_Standard_Deviation") {
          anytransaction = true;
          // Set the text content to key-value pairs
          listItem.textContent = `${key.replace(/_/g, ' ')}: ${value}`;

          // Append the list item to the UL
          statList.appendChild(listItem);
        }
        else if (key) {
          catvalues.push({ cat: key, spend: value });
        }
      });

      if (!anytransaction) {
        const listItem1 = document.createElement("li");
        listItem1.textContent = `Total Spend: Nil`;
        statList.appendChild(listItem1);
        const listItem2 = document.createElement("li");
        listItem2.textContent = `Month Average: Nil`;
        statList.appendChild(listItem2);
        const listItem3 = document.createElement("li");
        listItem3.textContent = `Month Standard Deviation: Nil`;
        statList.appendChild(listItem3);
      }

      if (statBarGraph) {
        statBarGraph.destroy(); // Destroy the existing chart
      }

      const category = catvalues.map(data => data.cat);
      const amounts = catvalues.map(data => data.spend);

      // Creating graph
      const ctx = document.getElementById("statBarGraph").getContext("2d");
      statBarGraph = new Chart(ctx, {
        type: 'bar',
        data: {
          labels: category,
          datasets: [{
            label: 'Category Expenditure',
            data: amounts,
            backgroundColor: 'rgba(185, 28, 28,0.9)',
            borderColor: 'rgba(255, 223, 0,0.7)',
            borderWidth: 2,
            hoverBackgroundColor: 'red', // Hover color
            hoverBorderColor: 'gold'
          }]
        },
        options: {
          responsive: true,
          scales: {
            y: {
              beginAtZero: true,
              title: {
                display: true,
                text: 'Amount (INR)',
                font: {
                  size: 14,
                },
                color: 'aliceblue'
              },
              grid: {
                color: 'rgba(255,255,255,0)',
                lineWidth: 1
              },
              ticks: {
                color: 'aliceblue',
                font: {
                  size: 12
                }
              }
            },
            x: {
              title: {
                display: true,
                text: 'Category',
                font: {
                  size: 14,
                },
                color: 'aliceblue'
              },
              grid: {
                color: 'rgba(255,255,255,0)',
                lineWidth: 1
              },
              ticks: {
                color: 'aliceblue',
                font: {
                  size: 12
                }
              }
            },
          },
          plugins: {
            legend: {
              labels: {
                color: 'aliceblue',
                font: {
                  size: 16
                }
              }
            }
          }
        }
      });
    })
    .catch(error => {
      console.error('Error fetching recommendations:', error);
    });
}                