#vertex_shader
 #version 330 core\n 
 layout (location = 0) in vec3 aPos;\n 
 void main()\n 
 {\n 
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n 
 }\0 ;
  
#fracment_shader
  #version 330 core\n
 out vec4 FragColor;\n 
 \n 
 void main() 
 {\n 
    FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n 
 };\0 ;