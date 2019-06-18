
function Quaternion(s, x, y, z) {
  this.s = s;
  this.x = x;
  this.y = y;
  this.z = z;

  return this;
}

Quaternion.fromEuler = function(rotX, rotY, rotZ) {

  // My version had bugs...
  var q = quat.create();
  quat.rotateX(q, q, rotX);
  quat.rotateY(q, q, rotY);
  quat.rotateZ(q, q, rotZ);
  quat.normalize(q,q);
  return new Quaternion(q[0], q[1], q[2], q[3]);
/*
  var xs = Math.sin(rotX),
      xc = Math.cos(rotX),
      ys = Math.sin(rotY),
      yc = Math.cos(rotY),
      zs = Math.sin(rotZ),
      zc = Math.cos(rotZ);
  var q =  new Quaternion(
      xc*yc*zc + xs*ys*zs,
      xc*ys*zc - xs*yc*zs,
      xc*yc*zs - xs*yc*zc,
      xs*yc*zc - xc*yc*zc
    );
  return q.normalize();
  */
}



Quaternion.prototype.multiply = function(q2) {
  var v1DotV2 = this.x*q2.x + this.y*q2.y + this.z*q2.z;
  var crossProductX = this.y*q2.z - this.z*q2.y,
      crossProductY = this.z*q2.x - this.x*q2.z,
      crossProductZ = this.x*q2.y - this.y*q2.x;
  return new Quaternion(this.s*q2.s - v1DotV2,
      this.s*q2.x + q2.s*this.x + crossProductX,
      this.s*q2.y + q2.s*this.y + crossProductY,
      this.s*q2.z + q2.s*this.z + crossProductZ).normalize();
}

Quaternion.prototype.add = function(q2) {
  return new Quaternion(this.s+q2.s, this.x+q2.x, this.y+q2.y, this.z+q2.z)
}

Quaternion.prototype.inverse = function() {
  return new Quaternion(-this.s, -this.x, -this.y, -this.z);
}

Quaternion.prototype.toArray = function() {
  var q = this;
  return [q.s, q.x, q.y, q.z];
}

Quaternion.prototype.toMatrix = function() {
  var q = this.normalize();

  var mat = []
  mat4.fromQuat(mat, q.toArray());
  return mat;

  // There's a bug here somewhere...
  /*

  return [
    1-2*q.y*q.y - 2*q.z*q.z, 2*q.x*q.y-2*q.z*q.s, 2*q.x*q.z+2*q.s*q.y, 0,
    2*q.x*q.y+2*q.s*q.z, 1-2*q.x*q.x-2*q.z*q.z, 2*q.y*q.z-2*q.s*q.x, 0,
    2*q.x*q.z-2*q.s*q.y, 2*q.y*q.z + 2*q.s*q.x, 1-2*q.x*q.x-2*q.y*q.y, 0,
    0,0,0,1
  ];
  */
}

Quaternion.prototype.length = function() {
  return Math.sqrt(this.s*this.s + this.x * this.x
               + this.y * this.y + this.z * this.z);
}

Quaternion.prototype.normalize = function() {
  var length = this.length();
  this.s = this.s/length;
  this.x = this.x/length;
  this.y = this.y/length;
  this.z = this.z/length;
  return this;
}
