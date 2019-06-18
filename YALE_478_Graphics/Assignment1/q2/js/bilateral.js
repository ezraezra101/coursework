
// bilateral filtering!

// n = filter size, m = filter threshold
function BilateralFilter(imageData, imageOutData, n, m) {
	this.n = n;
	this.m = m;
	this.imageData = imageData;
	this.imageOutData = imageOutData;
}

BilateralFilter.prototype.index = function(i,j) {
	return i*4*this.imageData.width + j*4;
}

BilateralFilter.prototype.pixelIntensity = function(i,j) {
	var idx = this.index(i,j);

	if(Math.random() > 0.999) {
		console.log(i,j, this.imageData.data[idx]);
	}
	return this.imageData.data[idx]*0.3
	 + this.imageData.data[idx+1]*0.5
	 + this.imageData.data[idx+2]*0.2;
}

BilateralFilter.prototype.filterPixel = function(i, j) {
	var intensity =  this.pixelIntensity(i,j);
	var count = 0;
	var results = [0,0,0,0];

	for(var k= -this.n; k <= this.n; k++) {
		for(var l= -this.n; l <= this.n; l++) {
			if(intensity <= this.pixelIntensity(k+i, l+j) + this.m &&
				 intensity >= this.pixelIntensity(k+i, l+j) - this.m) {
				var idx = this.index(k+i, l+j);
				for(var color = 0; color < 4; color++) {
					results[color] += this.imageData.data[idx+color];
				}
				count += 1;
			}
		}
	}
	for(var color = 0; color < 4; color++) {
		results[color] /= count;

		this.imageOutData.data[this.index(i,j) + color] = results[color];
	}
}

BilateralFilter.prototype.filter = function() {
	for (var j=0; j<this.imageData.width; j++)
  {
    for (var i=0; i<this.imageData.height; i++)
    {
    	this.filterPixel(i,j);

    }
  }
}