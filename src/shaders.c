#include "defs.h"

char* load_shader_file(const char *filename)
{
  assert(filename != NULL);

  FILE *file = fopen(filename, "rb");
  if(!file) {return NULL;}

  fseek(file, 0, SEEK_END);
  long lSize = ftell(file);
  rewind(file);

  char *buffer = calloc(1, lSize+1);
  if(!buffer)
  {
    fclose(file);
    return NULL;
  }

  if(1 != fread(buffer, lSize, 1, file))
  {
    fclose(file);
    free(buffer);
    return NULL;
  }

  fclose(file);
  return buffer;
}

int create_shader(const char* filename, int type)
{
  assert(filename != NULL);
  
  const char* shader_string = load_shader_file(filename);
  if(shader_string == NULL)
  {
    print_log("ERROR: Failed to load %s\n", filename);
    return -1;
  }

  int params = GL_TRUE;
  GLuint s = glCreateShader(type);
  glShaderSource(s, 1, &shader_string, NULL);
  glCompileShader(s);
  glGetShaderiv(s, GL_COMPILE_STATUS, &params);
  if(GL_TRUE != params)
  {
    print_log("ERROR: GL shader index %i did not compile.\n", s);
    print_log_shader_info(s);
    return -2;
  }

  return s;
}
