// A hacked together OBJ reader.
// Will fail on any number of problems (e.g a non-triangulated/quads mesh, etc.)

function getNormal(p1, p2, p3) {
  var u = vec3.subtract([], p2, p1);
  var v = vec3.subtract([], p3, p1);
  var n = [
    u[1]*v[2] - u[2]*v[1],
    u[2]*v[0] - u[0]*u[2],
    u[0]*v[1] - u[1]*v[0],
  ];
  return n;
}

function parseOBJFile(text, usesQuads) {
  var pointsPerFace = 3;
  if(usesQuads) {
    pointsPerFace = 4;
  }
  var objects = text.split('\no ');

  // Remove leading comments + setup
  objects.shift();

  var verticesCount = 0;

  return objects.map(function(objectText) {
    var lines = objectText.split('\n');
    var objectName = lines[0];
    var vertices = lines.filter(function(line) {
      return /^\s*v /i.test(line);
    });
    var faces = lines.filter(function(line) {
      return /^\s*f /i.test(line);
    });
    var position = [];
    var indices = [];
    vertices.forEach(function(v) {
      var regex = /[+-]?\d+(\.\d+)?/g;
      var floats = v.match(regex).map(function(x) { return parseFloat(x); });
      if (floats.length != 3) {
      	console.log("Error loading obj file: detected a vertex with the wrong number of floats");
      }
      position = position.concat(floats);
    });

    faces.forEach(function(f) {
      var regex = /[+-]?\d+(\.\d+)?/g;
      // We want to index at 0, so we -1
      var floats = f.match(regex).map(function(x) { return parseInt(x)-1 - verticesCount; });

      // Ignore texture coordinates
      if(floats.length == 6) {
        floats = [floats[0], floats[2], floats[4]];
      }
      if(floats.length == 8) {
        floats = [floats[6], floats[4], floats[2], floats[6], floats[2], floats[0]];
      }

      if (floats.length != 3 && !usesQuads) {
      	console.log("Error loading obj file: detected a face with the wrong number of integers");
        console.log(f, floats);
      }
      indices = indices.concat(floats);

    });
    //verticesCount = verticesCount + position.length/3;

    return {
      name: objectName,
      position: position,
      indices: indices,
    };
  });
}

// Synchronously load OBJ file
function readOBJ(usesQuads) {
  return parseOBJFile(document.getElementById('OBJ').value, usesQuads)


	// Commented out: AJAX doesn't work on local files, so I'm using the dinky hidden input method.
	/*
	var url = window.location.protocol +
		window.location.hostname +
		window.location.pathname.replace(new RegExp('/[^/]+$'), '/SimpleBird.obj');		;
	var obj;
	var req = new XMLHttpRequest();
	req.onreadystatechange = function() {
		if(req.readyState === XMLHttpRequest.DONE) {
			if(req.status === 200) {
				obj = parseOBJFile(req.responseText);
			} else {
				console.log("Error loading OBJ file from " + url);
				obj = {};
			}
		}
	}
	// Note: this is synchronous! (This is probably bad coding, but works well in this case)
	req.open('GET', url, false);
	req.send();
	console.log(obj);
	return obj;

	*/
}
