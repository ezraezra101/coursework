function distToLine(xy,normal) {
    // Assuming line passes through 0,0.
    return Math.abs(xy[0]*normal[0] +xy[1]*normal[1]);
}

function falloff(t) {
    t = Math.abs(t);
    return t > 1 ? 0 : 1-(3-2*t)*t*t;
}

function brush3()
{
    this.init();
}

brush3.prototype =
{
    prevX: null, prevY: null,
    noise: null,
    init: function()
    {
        this.noise = new Noise(10,1/10);
    },

    destroy: function()
    {
    },

    strokeStart: function( x, y )
    {
      this.prevX = x;
      this.prevY = y;
    },

    stroke: function( x, y)
    {
        if(x==this.prevX && y == this.prevY) {
            return;
        }
        strokeContext.globalCompositeOperation = 'source-over';
        strokeContext.lineWidth = BRUSH_SIZE;
        strokeContext.strokeStyle = "rgba(" + BRUSH_COLOR[0] + ", " + BRUSH_COLOR[1] + ", " + BRUSH_COLOR[2] + ", " + 0.05 * BRUSH_PRESSURE + ")";

        var mins = [Math.min(x,this.prevX), Math.min(y,this.prevY)];
        var maxes = [Math.max(x,this.prevX), Math.max(y,this.prevY)];
        // Note: I'm not worrying about being out of bounds!
        var leftCorner = [
            Math.floor(mins[0]-BRUSH_SIZE),
            Math.floor(mins[1]-BRUSH_SIZE)
        ];
        var dims = [
            Math.ceil(2*BRUSH_SIZE+maxes[0]-mins[0]),
            Math.ceil(2*BRUSH_SIZE+maxes[1]-mins[1])
        ];
        var dist = Math.sqrt(Math.pow(x-this.prevX,2) + Math.pow(y-this.prevY,2));
        var normal = [
           -(this.prevY-y)/dist,
            (this.prevX-x)/dist,
        ];
        var im = strokeContext.getImageData(leftCorner[0],leftCorner[1], dims[0],dims[1]);

        for(var i=0; i<dims[0]; i++) {
            for(var j=0; j<dims[1]; j++) {

                var d = distToLine([i-BRUSH_SIZE+mins[0]-this.prevX,j-BRUSH_SIZE+mins[1]-this.prevY], normal);
                
                // If a point is beyond the edge of the line segment, use the distance to the end of the segment.
                var dPoint = Math.sqrt(Math.pow(i+leftCorner[0]-x,2) + Math.pow(j+leftCorner[1]-y,2));
                var dPrev  = Math.sqrt(Math.pow(i+leftCorner[0]-this.prevX,2) + Math.pow(j+leftCorner[1]-this.prevY,2));
                if(dPoint > dPrev) {
                    if(0< normal[1]*(i+leftCorner[0]-this.prevX) - normal[0]*(j+leftCorner[1]-this.prevY)) {
                        d = dPrev;
                    }
                } else {
                    if(0> normal[1]*(i+leftCorner[0]-x) -normal[0]*(j+leftCorner[1]-y)) {
                        d = dPoint;
                    }
                }
                var f = falloff(0.5*d/BRUSH_SIZE + 0.5*d/(BRUSH_SIZE*this.noise.noise(i+leftCorner[0],j+leftCorner[1])));
                if(f > 0) {
                    im.data[4*i+4*j*dims[0]+0] = BRUSH_COLOR[0];
                    im.data[4*i+4*j*dims[0]+1] = BRUSH_COLOR[1];
                    im.data[4*i+4*j*dims[0]+2] = BRUSH_COLOR[2];
                    im.data[4*i+4*j*dims[0]+3] += 100*f;
                }
            }
        }
        strokeContext.putImageData(im, leftCorner[0], leftCorner[1]);

        this.prevX = x;
        this.prevY = y;

    },

    strokeEnd: function()
    {

    }
}
