// JavaScript for Name Data Lookup Demo
// Jim Skon, Kenyon College, 2019
var searchType;  // Save search type here
const baseUrl = 'http://3.134.78.249:5002';

console.log("Start!");
searchType="Last";
// Add a click event for the search button
document.querySelector("#search-btn").addEventListener("click", (e) => {
    getMatches();
});

// Add an event listener for each item in the pull down menu
document.querySelectorAll('.dropdown-menu a').forEach(item => {
    item.addEventListener('click', event => {
	element = event.target;
	searchType=element.textContent;
	console.log("pick: "+searchType);
	// Get the pulldown parent
	pullDown = element.parentElement.parentElement;
	// Get and set the selection displayed
	selection = pullDown.querySelectorAll(".selection")[0];
	selection.innerHTML = searchType;
	
    })
})

// Build output table from list of name data
function nameTable(data) {
    nameList = data['results'];
    console.log(nameList);
    var table = '<table class="w3-table-all w3-hoverable" border="2"><tr><th>Name</th><th>%</th><th>Rank</th><tr>';
    nameList.forEach(function (e,i) {
	table += "<tr><td>"+e["name"]+"</td><td>"+e["percent"]+"</td><td>"+e["rank"]+"</td></tr>";
    });
    table += "</table>";

    return table;
}


function processResults(results) {
    document.querySelector('#searchresults').innerHTML = nameTable(results);
}

function clearResults() {
    document.querySelector('#searchresults').innerHTML = "";
}

function getMatches(){
    console.log("getMatches!");
    var searchStr=document.querySelector('#search').value;
    console.log(searchStr+":"+searchType);

    // Ignore short requests
    if (searchStr.length < 2) return;

    // Clear the previous results
    document.querySelector('#searchresults').innerHTML = "";

    fetch(baseUrl+'/name/'+searchStr+"/"+searchType, {
	method: 'get'
    })
	.then (response => response.json() )
        .then (data => processResults(data))
	.catch(error => {
	    {alert("Error: Something went wrong:"+error);}
	})
}


