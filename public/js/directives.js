'use strict';

/* Directives */


angular.module('StanWeb.directives', [])
  .directive('appVersion', ['version', function(version) {
    return function(scope, elm, attrs) {
      elm.text(version);
    };
  }])
  .directive('chart', function() {

    var size = 400;
    var x = d3.scale.linear()
                    .domain([-1,1])
                    .range([0,size]);
    var y = d3.scale.linear()
                    .domain([-1,1])
                    .range([size,0]);

    var max_samples = 5;
    var samples = [];
    
    function redraw(chart) {
      var lines = chart.selectAll("line")
        .data(samples);
      
      lines.enter().append("line");
      var lineAttributes 
        = lines
        .attr("x1", x(-1))
        .attr("y1", function(d) {return y(d.alpha - d.beta); })
        .attr("x2", x(1))
        .attr("y2", function(d) {return y(d.alpha + d.beta); })
        .style("stroke-width", 1)
        .style("stroke", "blue")
        .style("opacity", function(d,i) {return ((i+1) / samples.length)});
      lines.exit().remove();
    }
    // define constants and helpers used for the directive
    // ...
    return {
      restrict: 'E', // the directive can be invoked only by using <my-directive> tag in the template
      scope: { // attributes bound to the scope of the directive
        val: '='
      },
      link: function (scope, element, attrs) {
        var chart = d3.select(element[0])
          .append("svg")
          .attr("width", size)
          .attr("height", size)
          .style("border", "1px solid black");
        
        scope.$watch('val', function(newVal, oldVal) {
          var sample = {
            alpha: newVal.sample[0],
            beta: newVal.sample[1],
            sigma: newVal.sample[2]
          };
          samples.push(sample);
          if (samples.length > max_samples) {
            samples.shift();
          }
          redraw(chart);
        });

      }
    };
  })
