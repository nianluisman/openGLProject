#include "Prism.h"

void Prism::setBuffer() {
	// Set up the vertex attribute pointers

	glGenBuffers(1, &VBOPrism);//make data buffer
	glGenBuffers(1, &EBOPrism);// index buffer 
	glGenVertexArrays(1, &VAOprism);//

	glBindVertexArray(VAOprism);

	glBindBuffer(GL_ARRAY_BUFFER, VBOPrism);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOPrism);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}