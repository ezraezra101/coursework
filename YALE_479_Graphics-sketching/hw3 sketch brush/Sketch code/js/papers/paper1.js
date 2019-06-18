function shuffle(a) {
  //Fisher–Yates shuffle
  for(var i=a.length-1; i>0;i--) {
    var j=Math.floor(Math.random()*i);
    var x = a[j];
    a[j] = a[i];
    a[i] = x;
  }
  return a;
}

function permutations(length) {
  a=[];
  for(var i=0;i<length;i++){
    a[i]=i;
  }
  return shuffle(a);
}

function gradsTable(length) {

  var grads = [];
  for(var i=0;i<length;i++) {
    grads[i] = [
      Math.cos(2*Math.PI*i/length),
      Math.sin(2*Math.PI*i/length)
    ];
  }
  return grads;
}


// Perlin noise
function Noise(length, scale) {
  this.perm_x = permutations(length);
  this.perm_y = permutations(length);
  this.grads = gradsTable(length);
  this.scale = scale;
}

Noise.prototype = {
  grads: null,
  scale: null,
  perm_x: null, perm_y: null,

  hash: function(x,y) {
    // X and Y are non-negative integers
    // Produces an  integer [0,length) based on x and y.
    // Note: may be strange for negative numbers
    return (this.perm_x[ x % this.perm_x.length ] ^ this.perm_y[ y % this.perm_y.length ])%this.perm_x.length;
  },

  falloff: function(t) {
    t = Math.abs(t);
    return t > 1 ? 0 : 1-(3-2*t)*t*t;
  },

  surflet: function(x,y, grad) {
    return this.falloff(x)*this.falloff(y)*(grad[0]*x+grad[1]*y);
  },

  noise: function(x,y) {
    x = this.scale*x; y = this.scale * y;
    var that = this;
    var result = 0;
    
    var xy = [Math.floor(x), Math.floor(y)];

    [0,1].forEach(function(y_edge) {
      [0,1].forEach(function(x_edge) {
        var h = that.hash(x_edge+xy[0], y_edge+xy[1]);
        result += that.surflet(x-(xy[0]+x_edge), y-(xy[1]+y_edge), that.grads[h]);
      });
    });
    return result;
  },
};



// This is going to be a runny paper.
function paper1()
{
  this.init();
}

paper1.prototype =
{
  kernel: null,
  noise: null,
  noiseArray: null,

  init: function()
  {
    var kernel = [];
    var kernelLength = 15;
    var stdDev = kernelLength/5;
    var divisor = 2*stdDev*stdDev;
    for(var i=0; i<kernelLength;i++) {
      var x = i-kernelLength/2;
      kernel[i] = Math.pow(Math.E,-(x*x/divisor));
    }
    this.kernel = kernel;

    this.noise = new Noise(1024,1/5);


    this.noiseArray = [];
    for(var j=0; j<screenHeight; j++) {
      var row = [];
      for(var i=0; i<screenWidth; i++) {
        row.push(this.noise.noise(i,j));
      }
      this.noiseArray.push(row);
    }
  },

  destroy: function()
  {
  },

  apply: function(stroke, paper)
  {
    if (stroke) 
    {
      var strokes = strokeContext.getImageData(0, 0, screenWidth, screenHeight);
      var temp = new Uint8ClampedArray(strokes.data.length);
      var pixels = new Uint8ClampedArray(strokes.data.length);
      

      for (var j = 0; j < screenHeight; ++j)
      {
        for (var i = 0; i < screenWidth; ++i)
        {
          var idx = (j*screenWidth + i)*4;
          if (strokes.data[idx + 3] > 0) {
            for(var k =0; k < this.kernel.length; k++) {              
              var offset = idx+4*(k-Math.floor(this.kernel.length/2))*screenWidth;
              temp[offset+0] += strokes.data[idx+0] * this.kernel[k];
              temp[offset+1] += strokes.data[idx+1] * this.kernel[k];
              temp[offset+2] += strokes.data[idx+2] * this.kernel[k];
              temp[offset+3] += this.kernel[k] * strokes.data[idx+3];
            }
          }
        }
      }

      for (var j = 0; j < screenHeight; ++j)
      {
        for (var i = 0; i < screenWidth; ++i) 
        {
          var idx = (j*screenWidth + i)*4;
          if (temp[idx + 3] > 0) {
            for(var k =0; k < this.kernel.length; k++) {
              var rgb = [temp[idx],temp[idx+1], temp[idx+2]];
              
              var offset = idx+4*(k-Math.floor(this.kernel.length/2))
              pixels[offset+0] += this.noiseArray[j][i] * this.kernel[k] /* rgb[0]*/*CANVAS_COLOR[0];
              pixels[offset+1] += this.noiseArray[j][i] * this.kernel[k] /* rgb[0]*/*CANVAS_COLOR[1];
              pixels[offset+2] += this.noiseArray[j][i] * this.kernel[k] /* rgb[0]*/*CANVAS_COLOR[2];
              pixels[offset+3] += this.noiseArray[j][i] * this.kernel[k] * temp[idx+3];
            }
          }
        }
      }
      for(var i=0; i<strokes.data.length/4; i++) {
        if(strokes.data[4*i+3] != 0) {
          pixels[4*i+0] = (pixels[4*i+0] + strokes.data[4*i+0])/2;
          pixels[4*i+1] = (pixels[4*i+1] + strokes.data[4*i+1])/2;
          pixels[4*i+2] = (pixels[4*i+2] + strokes.data[4*i+2])/2;
          pixels[4*i+3] = (pixels[4*i+3] + strokes.data[4*i+3])/2;
        }
      }

      strokes.data.set(pixels);
      context.putImageData(strokes, 0, 0);
    }
    if (paper) 
    { // fill the paper with the selected canvas color
      bgContext.fillStyle = 'rgb(255,255,255)';// + CANVAS_COLOR[0] + ', ' + CANVAS_COLOR[1] + ', ' + CANVAS_COLOR[2] + ')';
      bgContext.fillRect(0, 0, screenWidth, screenHeight);        
    }
  }
};
