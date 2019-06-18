// A file devoted to reading in the spline file into a javascript object

// Parses in a spline file of the form described in spline1.txt.
// returns an object in the form of:
// [{ // For each spline
//    time: float,
//    points: [{
//        position: [x, y, z]
//        rotation: [x, y, z]
//      }, ...]
// }, ...]
function parseSplineFile(text) {
  var lines = text.split('\n').filter(function(line) {
    return line[0] != '#' && line != '';
  });

  var nSplines = parseInt(lines.shift());

   return Array(nSplines).fill("").map(function() {
     // For each spline: (note: fill("") is totally necessary. map doesn't run otherwise)

     var nPoints = parseInt(lines.shift());
     return {
       time: parseFloat(lines.shift()),
       points: Array(nPoints).fill("").map(function() {
         return {
           position: lines.shift().split(',').map(parseFloat),
           rotation: lines.shift().split(',').map(parseFloat)
         };
       })
     };
   });
}

// Hunts down a spline file and parses it
function readSpline() {
  return parseSplineFile(document.getElementById('spline').value)
  // var reader = new FileReader();
  // reader.onload = function() {
  //   var text = reader.result;
  //   callback(parseSplineFile(text));
  // }

}
