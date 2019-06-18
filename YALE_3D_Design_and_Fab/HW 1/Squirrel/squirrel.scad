$fn=5;

/*
// minkowski sum aka dilation = "replace every single point on the cube with a sphere"... convolution
rotate([47,2,3]){
minkowski() {
    translate([1,0.23,0]){
        cube([2,3,4]);
    };
    sphere(0.5);
}
}

sphere(0.5);

*/

module tail(steps, angles) {
    // function tailRadius(x=0) = x-pow(x,3);
    function tailRadius(x=0) = 1-x-pow(1-x,3);

    // The line that the center of the tail follows
    function tailCenter(x) = sin(180 * x)/ 2 + 0.5 * x;
    
    function tailLocation(step, angle) = [
        sin(angle) * tailRadius(step),
        cos(angle) * tailRadius(step) + tailCenter(step),
        step];
    
    for(i=[0:steps]) {
        for(j=[0:angles-1]) {
            translate(tailLocation(i/steps, (i/steps + j/angles)*360))
            sphere(0.1, $fn=4);
        }
    }
}

rotate([40,0,0])
//hull()
tail(30, 7);
//sphere(1);
//tail();


/*
module tail_hair() {
        translate([0,1,0])
    linear_extrude(height = 5, center = true, convexity = 10, twist = -120)
    translate([.5, 0, 0])
    circle(r = .1);
};
*/

/*
$fn=20;

scale(0.25)
translate([0,0,1.5])
rotate([0,0,0]){
    scale([1,1,2])
    sphere(1);
    
    translate([0,-.3,1])
    rotate([30,0,0]) {
        scale([1.1,1,2])
        sphere(1);
        
        translate([0,-0.3,1.5])
        
        scale([1,1.2,1])
        sphere(1);
        
        tail();
    }
}
*/
$fn=20;
module body() {
    // Arms
    translate([0.15,-0.5,0.45]) {
        rotate([0, -25, -20]) {
            rotate([20,0,0])
            scale([0.1, 0.25, 0.15])
            sphere(1);
            
            translate([0.0, -0.25, -0.1])
            scale([0.05, 0.15, 0.07])
            sphere(1);
            
            translate([0.0, -0.35, -0.1])
            scale([0.05, 0.07, 0.07])
            sphere(1);
        }
    }
    
    // Legs
    translate([0.2,-0.25,0]) {
        translate([0,0,0.1])
        scale([0.1, 0.2, 0.3])
        sphere(1);
        
        translate([0.0, -0.1, -0.2])
        scale([0.1, 0.2, 0.05])
        sphere(1);
    }
    
    // Body
    translate([0,-0.2,0.3]) {
        rotate([20,0,0])
        scale([.25,.25,.5])
        sphere(1);
        
    }
    
    // Head + neck
    scale([0.9,1,1])
    translate([0,-0.33,0.65]) {
        // Head
        translate([0,-0.13,0.15])
        rotate([15,0,0]) {
            scale([0.18, 0.22, 0.16])
            sphere(1);
            
            // Snout
            rotate([5,0,0]){
                translate([0,-0.11, -0.02])
                scale([0.1, 0.2, 0.07])
                sphere(1);
                translate([0,-0.07, -0.05])
                scale([0.1, 0.2, 0.05])
                sphere(1);
            }
            
            // Eye
            translate([0.15,-0.05,0.05])
            scale(0.05)
            sphere(1);
            
            // Ear
            translate([.1,0.1,.1])
            rotate([-20,0,0])
            scale(.3) {
                scale([0.15, 0.2, 0.5]) {
                    difference() {
                        sphere(1);
                        translate([0.3,0,0])
                        sphere(0.85);
                    }
                }
            }
        }
        
        // Neck
        translate([0, -0.01, 0.0])
        scale(0.12)
        rotate([20,0,0])
        cylinder(1);
    }
}

difference() {

    body();
    
    translate([0.26, -0.43, 0])
    rotate([90,0,90])
    scale(.02)
    linear_extrude(height = 2)
    text("ED");
}
mirror() body();
