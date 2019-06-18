
class Turtle {
public:
  Turtle(const char * filename);
  static void initialize();
  void draw(mat4 ctm, GLuint program);

  static void drawSphere(mat4 ctm, point4 position, float size, GLuint program);
  static void drawCylinder(mat4 ctm, point4 start, point4 end, float size, GLuint program);
  static void drawPolyCylinder(mat4 ctm, point4 start, point4 end, GLuint program);
private:
  void assemble(mat4 ctm, GLuint program);
  string pattern;
  Grammar grammar;
  static GLuint buffer; //VBO that the cylinder is drawn in
  static GLuint program;
};
