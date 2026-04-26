$(function() {
    var resize_timeout = null;
    $(window).resize(function() {
        if (resize_timeout) clearTimeout(resize_timeout);
        resize_timeout = setTimeout(function() {
            drawChart();
        }, 20);
    });
});

google.charts.load("current", {packages:["calendar"]});
google.charts.setOnLoadCallback(drawChart);
function drawChart() {
    var dataTable = new google.visualization.DataTable();
    dataTable.addColumn({ type: 'date', id: 'Date' });
    dataTable.addColumn({ type: 'number', id: 'Submit' });
    var ans = [];
    var check = {}; 
    user_day_problems.forEach(function(e) {
        var year = Math.floor(e[0]/10000);
        var month = Math.floor((e[0]%10000)/100);
        var day = e[0]%100;
        ans.push([new Date(year,month-1,day), e[1]]);
        check[year] = 1;
    });
    var cnt = 0;
    if (start_year in check) {
        cnt += 1;
    }
    if (end_year in check) {
        cnt += 1;
    }
    if (cnt == 0) cnt = 1;
    dataTable.addRows(ans);

    var div = document.getElementById('heatmap');

    var chart = new google.visualization.Calendar(div);

    var cell_size = div.clientWidth/58;
    var height = 20+30*cnt+cell_size*7*cnt;

    var options = {
        height: height,
        colorAxis: {
            minValue: 0,
            colors: ['#aff59a', '#009874']
        },
        calendar: {
            cellSize: cell_size,
        }
    };

    chart.draw(dataTable, options);
}

