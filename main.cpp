#include <gl/glut.h>
#include <stdio.h>
bool esp=false;
bool mostrar=true;
int nivel = 0;
GLfloat angulo=0.0;
float posX = -700.0f;
float posY = -700.0f;
float ySol = -380.0f;
float di = 1.0f;
int vidatotal = 3;

typedef struct {
	GLubyte *dibujo;
	GLuint bpp;
	GLuint largo;
	GLuint ancho;
	GLuint ID;
}textura;

textura ini;
textura Nivel;
textura Nivel2;
textura Nivel3;
textura Nivel4;
textura axu;
textura animal2;
textura corazon;
textura derrota;

int cargaTGA(char const*nombre, textura *imagen){
	GLubyte cabeceraTGA[12]={0,0,2,0,0,0,0,0,0,0,0,0};
	GLubyte compararTGA[12];
	GLubyte cabecera[6];
	GLuint bytesporpunto;
	GLuint tamanoimagen;
	GLuint temp,i;
	GLuint tipo=GL_RGBA;
	
	FILE *archivo=fopen(nombre,"rb");
	if(archivo==NULL || fread(compararTGA,1,sizeof(compararTGA),archivo) != sizeof(compararTGA) || memcmp(cabeceraTGA,compararTGA,sizeof(compararTGA)) != 0 || 
	fread(cabecera,1,sizeof(cabecera),archivo) != sizeof(cabecera) )
	{
		if(archivo == NULL){
			printf("No se encontro el archivo %s\n",nombre);
			return 0;
		}
		else{
			fclose(archivo);
			return 0;
		}
	}
	imagen->largo=256*cabecera[1]+cabecera[0];
	imagen->ancho=256*cabecera[3]+cabecera[2];
	
	if(imagen->largo<=0 || imagen->ancho<=0 || (cabecera[4] != 24 && cabecera[4] != 32)){
		printf("Datos invalidos\n");
		fclose(archivo);
		return 0;
	}
	
	imagen->bpp=cabecera[4];
	bytesporpunto=cabecera[4]/8;
	tamanoimagen=imagen->largo * imagen->ancho * bytesporpunto;
	
	imagen->dibujo = (GLubyte *)malloc(tamanoimagen);
	if(imagen->dibujo == NULL || fread(imagen->dibujo,1,tamanoimagen,archivo) != tamanoimagen){
		if(imagen->dibujo != NULL){
			printf("Error leyendo imagen \n");
			free(imagen->dibujo);
		}
		else{
			printf("Error asignando memoria \n");
		}
		fclose(archivo);
		return 0;
	}
	
	for(i=0;i<(int)tamanoimagen; i+=bytesporpunto){
		temp=imagen->dibujo[i];
		imagen->dibujo[i]=imagen->dibujo[i+2];
		imagen->dibujo[i+2]=temp;
	}
	
	fclose(archivo);
	glGenTextures(1,&imagen[0].ID);
	glBindTexture(GL_TEXTURE_2D,imagen[0].ID );
	
	if(imagen->bpp == 24) tipo=GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, tipo, imagen[0].ancho, imagen[0].largo, 0, tipo, GL_UNSIGNED_BYTE, imagen[0].dibujo);
	
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	return 1;
}

void init(){
	
	GLfloat light_position[]={0,0,1,0};
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0,GL_POSITION,light_position);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	
	glShadeModel(GL_SMOOTH); //sombras
	glClearColor(0.0,0.0,0.0,0.0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_TEXTURE_2D);
	//CARGA DE TEXTURA
	if(!cargaTGA("zelda1.tga",&Nivel)){
		printf("Error cargando textura \n");
		exit(0);
	}
	if(!cargaTGA("zelda2.tga",&Nivel2)){
		printf("Error cargando textura \n");
		exit(0);
	}
	if(!cargaTGA("zelda3.tga",&Nivel3)){
		printf("Error cargando textura \n");
		exit(0);
	}
	if(!cargaTGA("zelda4.tga",&Nivel4)){
		printf("Error cargando textura \n");
		exit(0);
	}
	if(!cargaTGA("inicio.tga",&axu)){
		printf("Error cargando textura \n");
		exit(0);
	}
	if(!cargaTGA("inicio.tga",&ini)){
		printf("Error cargando textura \n");
		exit(0);
	}
	if(!cargaTGA("corazones.tga",&corazon)){
		printf("Error cargando textura \n");
		exit(0);
	}
	if(!cargaTGA("figura2.tga",&animal2)){
		printf("Error cargando textura \n");
		exit(0);
	}
	if(!cargaTGA("game_over.tga",&derrota)){
		printf("Error cargando textura \n");
		exit(0);
	}
}


void fondo(textura tex){
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,tex.ID);
	int t = 600;
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3d(t, t, -t);
	glTexCoord2f(1.0, 0.0); glVertex3d(-t, t, -t);
	glTexCoord2f(1.0, 1.0); glVertex3d(-t, -t, -t);
	glTexCoord2f(0.0, 1.0); glVertex3d(t, -t, -t);
	
	glEnd();
	glLoadIdentity();
	
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
}


void soldado(float xPos){
	
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,animal2.ID);
	int t = 50;
	ySol += 0.03f * di;
	float iSol = -380.0f;
    float fSol = 320.0f;
    
	if (ySol >= fSol || ySol <= iSol) {
        di *= -1;
    }
    
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3d(t+xPos, t+ySol, -t);
	glTexCoord2f(1.0, 0.0); glVertex3d(-t+xPos, t+ySol, -t);
	glTexCoord2f(1.0, 1.0); glVertex3d(-t+xPos, -t+ySol, -t);
	glTexCoord2f(0.0, 1.0); glVertex3d(t+xPos, -t+ySol, -t);
	
	glEnd();
	glLoadIdentity();
	
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
}

void vida(float x, float y){
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,corazon.ID);
	int t = 15;
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3d(t+x, t+y, -t);
	glTexCoord2f(1.0, 0.0); glVertex3d(-t+x, t+y, -t);
	glTexCoord2f(1.0, 1.0); glVertex3d(-t+x, -t+y, -t);
	glTexCoord2f(0.0, 1.0); glVertex3d(t+x, -t+y, -t);
	
	glEnd();
	glLoadIdentity();
	
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
}

void enemigo(float x, float y, float R1, float R2, float R3, float R4, float vel){
	static float ejeY = y;
    static bool mov = true;
	float c = 0.1;
	glDisable(GL_LIGHTING);
	glTranslatef(x, ejeY, 0.0);
	
	//color claro
	glColor3f(R2, R3, R4);
	glBegin(GL_QUADS);
		glVertex3d(-200*c,65*c,0);
		glVertex3d(160*c,60*c,0);
		glVertex3d(160*c,-68*c,0);
		glVertex3d(-200*c,-70*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(-155*c,-70*c,0);
		glVertex3d(112*c,-68*c,0);
		glVertex3d(112*c,-115*c,0);
		glVertex3d(-155*c,-115*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(-290*c,200*c,0);
		glVertex3d(250*c,200*c,0);
		glVertex3d(250*c,60*c,0);
		glVertex3d(-290*c,60*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(-110*c,330*c,0);
		glVertex3d(70*c,330*c,0);
		glVertex3d(70*c,290*c,0);
		glVertex3d(-110*c,290*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(-200*c,290*c,0);
		glVertex3d(160*c,290*c,0);
		glVertex3d(160*c,245*c,0);
		glVertex3d(-200*c,245*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(-248*c,245*c,0);
		glVertex3d(200*c,245*c,0);
		glVertex3d(200*c,200*c,0);
		glVertex3d(-248*c,200*c,0);
	glEnd();
	
		//blanco
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
		glVertex3d(-380*c,65*c,0);
		glVertex3d(-340*c,65*c,0);
		glVertex3d(-340*c,20*c,0);
		glVertex3d(-380*c,20*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(-380*c,290*c,0);
		glVertex3d(-340*c,290*c,0);
		glVertex3d(-340*c,332*c,0);
		glVertex3d(-380*c,332*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(290*c,60*c,0);
		glVertex3d(340*c,60*c,0);
		glVertex3d(340*c,20*c,0);
		glVertex3d(290*c,20*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(290*c,334*c,0);
		glVertex3d(340*c,334*c,0);
		glVertex3d(340*c,290*c,0);
		glVertex3d(290*c,290*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(-155*c,65*c,0);
		glVertex3d(112*c,65*c,0);
		glVertex3d(112*c,-25*c,0);
		glVertex3d(-155*c,-25*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(-110*c,-25*c,0);
		glVertex3d(70*c,-25*c,0);
		glVertex3d(70*c,-70*c,0);
		glVertex3d(-110*c,-70*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(-110*c,-115*c,0);
		glVertex3d(-65*c,-115*c,0);
		glVertex3d(-65*c,-160*c,0);
		glVertex3d(-110*c,-160*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(25*c,-115*c,0);
		glVertex3d(70*c,-115*c,0);
		glVertex3d(70*c,-160*c,0);
		glVertex3d(25*c,-160*c,0);
	glEnd();
	
	//color
	glColor3f(R1, 0.2f, 0.0f);
	glBegin(GL_QUADS);
		glVertex3d(-340*c,20*c,0);
		glVertex3d(-380*c,20*c,0);
		glVertex3d(-380*c,-210*c,0);
		glVertex3d(-340*c,-210*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(-380*c,112*c,0);
		glVertex3d(-340*c,112*c,0);
		glVertex3d(-340*c,290*c,0);
		glVertex3d(-380*c,290*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(290*c,-210*c,0);
		glVertex3d(340*c,-210*c,0);
		glVertex3d(340*c,20*c,0);
		glVertex3d(290*c,20*c,0);
	glEnd();
	
	
	glBegin(GL_QUADS);
		glVertex3d(290*c,110*c,0);
		glVertex3d(340*c,110*c,0);
		glVertex3d(340*c,290*c,0);
		glVertex3d(290*c,290*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(-340*c,65*c,0);
		glVertex3d(-290*c,65*c,0);
		glVertex3d(-290*c,20*c,0);
		glVertex3d(-340*c,20*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(-290*c,65*c,0);
		glVertex3d(-200*c,65*c,0);
		glVertex3d(-200*c,-25*c,0);
		glVertex3d(-290*c,-25*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(-340*c,332*c,0);
		glVertex3d(-248*c,332*c,0);
		glVertex3d(-248*c,290*c,0);
		glVertex3d(-340*c,290*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(-290*c,290*c,0);
		glVertex3d(-200*c,290*c,0);
		glVertex3d(-200*c,240*c,0);
		glVertex3d(-290*c,240*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(200*c,334*c,0);
		glVertex3d(290*c,334*c,0);
		glVertex3d(290*c,290*c,0);
		glVertex3d(200*c,290*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(160*c,290*c,0);
		glVertex3d(250*c,290*c,0);
		glVertex3d(250*c,245*c,0);
		glVertex3d(160*c,245*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(160*c,60*c,0);
		glVertex3d(250*c,60*c,0);
		glVertex3d(250*c,-25*c,0);
		glVertex3d(160*c,-25*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(250*c,60*c,0);
		glVertex3d(290*c,60*c,0);
		glVertex3d(290*c,20*c,0);
		glVertex3d(250*c,20*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(-110*c,290*c,0);
		glVertex3d(-65*c,290*c,0);
		glVertex3d(-65*c,245*c,0);
		glVertex3d(-110*c,245*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(25*c,290*c,0);
		glVertex3d(70*c,290*c,0);
		glVertex3d(70*c,245*c,0);
		glVertex3d(25*c,245*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(-200*c,155*c,0);
		glVertex3d(-155*c,155*c,0);
		glVertex3d(-155*c,112*c,0);
		glVertex3d(-200*c,112*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(112*c,155*c,0);
		glVertex3d(160*c,155*c,0);
		glVertex3d(160*c,112*c,0);
		glVertex3d(112*c,112*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(-65*c,65*c,0);
		glVertex3d(25*c,65*c,0);
		glVertex3d(25*c,-25*c,0);
		glVertex3d(-65*c,-25*c,0);
	glEnd();
	
	glEnable(GL_LIGHTING);
	
	if (mov) {
        ejeY -= vel;
        if (ejeY <= -420.f) {
            mov = false;
        }
    } else {
        ejeY += vel;
        if (ejeY >= y) {
            mov = true;
        }
    }
}

void Link(){
	float c = 0.2;
	
	glDisable(GL_LIGHTING);
	glTranslatef(posX, posY, 0.0f);
	if(esp){
		glColor3f(0.5f, 0.5f, 0.5f);
		glBegin(GL_QUADS);
			glVertex3d(230*c,-65*c,0);
			glVertex3d(350*c,-132*c,0);
			glVertex3d(430*c,135*c,0);
			glVertex3d(300*c,-95*c,0);
		glEnd();
	}
	
	//Sombrero
	glBegin(GL_QUADS);
		glVertex3d(230*c,-65*c,0);
		glVertex3d(220*c,-65*c,0);
		glVertex3d(250*c,-95*c,0);
		glVertex3d(180*c,-95*c,0);
	glEnd();
	
	glColor3f(0.71f, 0.86f, 0.42f);
	glBegin(GL_QUADS);
		glVertex3d(-90*c,260*c,0);
		glVertex3d(90*c,260*c,0);
		glVertex3d(90*c,200*c,0);
		glVertex3d(-90*c,200*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(-90*c,230*c,0);
		glVertex3d(-90*c,170*c,0);
		glVertex3d(-120*c,170*c,0);
		glVertex3d(-120*c,230*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(90*c,230*c,0);
		glVertex3d(90*c,170*c,0);
		glVertex3d(120*c,170*c,0);
		glVertex3d(120*c,230*c,0);
	glEnd();
	//Pelo
	glColor3f(0.83f, 0.34f, 0.27f);
	glBegin(GL_QUADS);
		glVertex3d(-90*c,200*c,0);
		glVertex3d(90*c,200*c,0);
		glVertex3d(90*c,140*c,0);
		glVertex3d(-90*c,140*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(-90*c,170*c,0);
		glVertex3d(-120*c,170*c,0);
		glVertex3d(-120*c,80*c,0);
		glVertex3d(-90*c,80*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(90*c,170*c,0);
		glVertex3d(120*c,170*c,0);
		glVertex3d(120*c,80*c,0);
		glVertex3d(90*c,80*c,0);
	glEnd();
	//Cara
	glColor3f(0.93f, 0.78f, 0.37f); 
	glBegin(GL_QUADS);
		glVertex3d(-180*c,203*c,0);
		glVertex3d(-150*c,203*c,0);
		glVertex3d(-150*c,80*c,0);
		glVertex3d(-180*c,80*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(-150*c,140*c,0);
		glVertex3d(-120*c,140*c,0);
		glVertex3d(-120*c,80*c,0);
		glVertex3d(-150*c,80*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(-150*c,80*c,0);
		glVertex3d(-90*c,80*c,0);
		glVertex3d(-90*c,50*c,0);
		glVertex3d(-150*c,50*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(180*c,203*c,0);
		glVertex3d(150*c,203*c,0);
		glVertex3d(150*c,80*c,0);
		glVertex3d(180*c,80*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(150*c,140*c,0);
		glVertex3d(120*c,140*c,0);
		glVertex3d(120*c,80*c,0);
		glVertex3d(150*c,80*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(150*c,80*c,0);
		glVertex3d(90*c,80*c,0);
		glVertex3d(90*c,50*c,0);
		glVertex3d(150*c,50*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(-90*c,140*c,0);
		glVertex3d(90*c,140*c,0);
		glVertex3d(90*c,25*c,0);
		glVertex3d(-90*c,25*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(-60*c,25*c,0);
		glVertex3d(-60*c,-5*c,0);
		glVertex3d(60*c,-5*c,0);
		glVertex3d(60*c,25*c,0);
	glEnd();
	
	//Ojos-Boca
	glColor3f(0.83f, 0.34f, 0.27f);
	glBegin(GL_QUADS);
		glVertex3d(-60*c,112*c,0);
		glVertex3d(-30*c,112*c,0);
		glVertex3d(-30*c,82*c,0);
		glVertex3d(-60*c,82*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(32*c,112*c,0);
		glVertex3d(60*c,112*c,0);
		glVertex3d(60*c,82*c,0);
		glVertex3d(32*c,82*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(-30*c,52*c,0);
		glVertex3d(-30*c,25*c,0);
		glVertex3d(30*c,25*c,0);
		glVertex3d(30*c,52*c,0);
	glEnd();
	
	glColor3f(0.71f, 0.86f, 0.42f);
	glBegin(GL_QUADS);
		glVertex3d(32*c,140*c,0);
		glVertex3d(60*c,140*c,0);
		glVertex3d(60*c,112*c,0);
		glVertex3d(32*c,112*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(-60*c,140*c,0);
		glVertex3d(-30*c,140*c,0);
		glVertex3d(-30*c,112*c,0);
		glVertex3d(-60*c,112*c,0);
	glEnd();
	
	//Verde
	glColor3f(0.71f, 0.86f, 0.42f);
	glBegin(GL_QUADS);
		glVertex3d(-150*c,50*c,0);
		glVertex3d(-90*c,50*c,0);
		glVertex3d(-90*c,25*c,0);
		glVertex3d(-150*c,25*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(90*c,50*c,0);
		glVertex3d(150*c,50*c,0);
		glVertex3d(150*c,25*c,0);
		glVertex3d(90*c,25*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(-28*c,-65*c,0);
		glVertex3d(0*c,-65*c,0);
		glVertex3d(0*c,-95*c,0);
		glVertex3d(-28*c,-95*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(31*c,-95*c,0);
		glVertex3d(150*c,-95*c,0);
		glVertex3d(150*c,-125*c,0);
		glVertex3d(31*c,-125*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(-28*c,-125*c,0);
		glVertex3d(120*c,-125*c,0);
		glVertex3d(120*c,-152*c,0);
		glVertex3d(-28*c,-152*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(123*c,25*c,0);
		glVertex3d(123*c,-5*c,0);
		glVertex3d(60*c,-5*c,0);
		glVertex3d(60*c,25*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(-28*c,-5*c,0);
		glVertex3d(123*c,-5*c,0);
		glVertex3d(123*c,-35*c,0);
		glVertex3d(-28*c,-35*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(31*c,-35*c,0);
		glVertex3d(90*c,-35*c,0);
		glVertex3d(90*c,-65*c,0);
		glVertex3d(-31*c,-65*c,0);
	glEnd();
	
	//Marron
	glColor3f(0.83f, 0.34f, 0.27f);
	glBegin(GL_QUADS);
		glVertex3d(-28*c,-35*c,0);
		glVertex3d(31*c,-35*c,0);
		glVertex3d(31*c,-65*c,0);
		glVertex3d(-28*c,-65*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(0*c,-65*c,0);
		glVertex3d(120*c,-65*c,0);
		glVertex3d(120*c,-95*c,0);
		glVertex3d(0*c,-95*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(-28*c,-95*c,0);
		glVertex3d(31*c,-95*c,0);
		glVertex3d(31*c,-125*c,0);
		glVertex3d(-28*c,-125*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(150*c,80*c,0);
		glVertex3d(180*c,80*c,0);
		glVertex3d(180*c,-35*c,0);
		glVertex3d(150*c,-35*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(180*c,25*c,0);
		glVertex3d(210*c,25*c,0);
		glVertex3d(210*c,-65*c,0);
		glVertex3d(180*c,-65*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(123*c,25*c,0);
		glVertex3d(155*c,25*c,0);
		glVertex3d(155*c,-5*c,0);
		glVertex3d(123*c,-5*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(-210*c,25*c,0);
		glVertex3d(-60*c,25*c,0);
		glVertex3d(-60*c,-5*c,0);
		glVertex3d(-210*c,-5*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(-240*c,-5*c,0);
		glVertex3d(-60*c,-5*c,0);
		glVertex3d(-60*c,-152*c,0);
		glVertex3d(-240*c,-152*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(-60*c,-5*c,0);
		glVertex3d(-28*c,-5*c,0);
		glVertex3d(-28*c,-35*c,0);
		glVertex3d(-60*c,-35*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(-60*c,-152*c,0);
		glVertex3d(-28*c,-152*c,0);
		glVertex3d(-28*c,-180*c,0);
		glVertex3d(-60*c,-180*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(-120*c,-180*c,0);
		glVertex3d(-28*c,-180*c,0);
		glVertex3d(-28*c,-210*c,0);
		glVertex3d(-120*c,-210*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(31*c,-152*c,0);
		glVertex3d(120*c,-152*c,0);
		glVertex3d(120*c,-182*c,0);
		glVertex3d(31*c,-182*c,0);
	glEnd();
	
	//Crema
	glColor3f(0.93f, 0.78f, 0.37f); 
	glBegin(GL_QUADS);
		glVertex3d(123*c,-5*c,0);
		glVertex3d(155*c,-5*c,0);
		glVertex3d(155*c,-35*c,0);
		glVertex3d(123*c,-35*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(90*c,-35*c,0);
		glVertex3d(185*c,-35*c,0);
		glVertex3d(185*c,-65*c,0);
		glVertex3d(90*c,-65*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(120*c,-65*c,0);
		glVertex3d(215*c,-65*c,0);
		glVertex3d(215*c,-95*c,0);
		glVertex3d(120*c,-95*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(150*c,-95*c,0);
		glVertex3d(185*c,-95*c,0);
		glVertex3d(185*c,-125*c,0);
		glVertex3d(150*c,-125*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(-60*c,-35*c,0);
		glVertex3d(-28*c,-35*c,0);
		glVertex3d(-28*c,-152*c,0);
		glVertex3d(-60*c,-152*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(-210*c,-152*c,0);
		glVertex3d(-60*c,-152*c,0);
		glVertex3d(-60*c,-180*c,0);
		glVertex3d(-210*c,-180*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(-210*c,-35*c,0);
		glVertex3d(-120*c,-35*c,0);
		glVertex3d(-120*c,-65*c,0);
		glVertex3d(-210*c,-65*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(-180*c,-5*c,0);
		glVertex3d(-150*c,-5*c,0);
		glVertex3d(-150*c,-35*c,0);
		glVertex3d(-180*c,-35*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(-180*c,-65*c,0);
		glVertex3d(-150*c,-65*c,0);
		glVertex3d(-150*c,-125*c,0);
		glVertex3d(-180*c,-125*c,0);
	glEnd();
	
	glTranslatef(-posX, -posY, 0.0f);
	glEnable(GL_LIGHTING);
}

void espada(){
	float c=0.1f;
	
	glDisable(GL_LIGHTING);
	glTranslatef(0.0f, -60.0F, 0.0f);
	glRotatef(angulo, 0.0f, 0.0f, 1.0f);
	
	glColor3f(0.886f, 0.360f, 0.067f);
	glBegin(GL_QUADS);
		glVertex3d(-30*c,535*c,0);
		glVertex3d(15*c,535*c,0);
		glVertex3d(15*c,485*c,0);
		glVertex3d(-30*c,485*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(-85*c,485*c,0);
		glVertex3d(68*c,485*c,0);
		glVertex3d(68*c,-20*c,0);
		glVertex3d(-85*c,-20*c,0);
	glEnd();
	
	glColor3f(0.733f, 0.961f, 0.102f);
	glBegin(GL_QUADS);
		glVertex3d(-185*c,-20*c,0);
		glVertex3d(170*c,-20*c,0);
		glVertex3d(170*c,-75*c,0);
		glVertex3d(-185*c,-75*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(-185*c,-75*c,0);
		glVertex3d(-135*c,-75*c,0);
		glVertex3d(-135*c,-125*c,0);
		glVertex3d(-185*c,-125*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(118*c,-75*c,0);
		glVertex3d(170*c,-75*c,0);
		glVertex3d(170*c,-125*c,0);
		glVertex3d(118*c,-125*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(-85*c,-125*c,0);
		glVertex3d(68*c,-125*c,0);
		glVertex3d(65*c,-175*c,0);
		glVertex3d(-85*c,-175*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(-85*c,-228*c,0);
		glVertex3d(65*c,-228*c,0);
		glVertex3d(68*c,-275*c,0);
		glVertex3d(-85*c,-275*c,0);
	glEnd();
	
	glColor3f(1.0f, 0.627f, 0.267f);
	glBegin(GL_QUADS);
		glVertex3d(-85*c,-75*c,0);
		glVertex3d(68*c,-75*c,0);
		glVertex3d(68*c,-125*c,0);
		glVertex3d(-85*c,-125*c,0);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3d(-85*c,-175*c,0);
		glVertex3d(65*c,-175*c,0);
		glVertex3d(65*c,-228*c,0);
		glVertex3d(-85*c,-228*c,0);
	glEnd();
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    fondo(axu);
    
    if(nivel == 1 || nivel == 2 || nivel == 3 || nivel == 4 || nivel == 5){
    	if(vidatotal == 3){
    		vida(450,500);
		    vida(500,500);
		    vida(550,500);
		}
		else if(vidatotal == 2){
			vida(450,500);
	    	vida(500,500);
		}
		else if(vidatotal == 1){
			vida(450,500);
		}
		else{
			axu=derrota;
			mostrar = false;
		}
	}
	if (nivel == 2){
		soldado(360.0f);
		soldado(-360.0f);
		espada();
		glLoadIdentity();
	}
	if (nivel == 5){
		soldado(360.0f);
		soldado(-360.0f);
		glLoadIdentity();
	}
	if(nivel == 3){
		enemigo(25.0f, 300.0f,0.0f,0.0f,0.8f,0.0f, 0.1f );
		glLoadIdentity();
		enemigo(-520.0f, 300.0f,0.0f,0.0f,0.8f,0.0f,0.08f);
		glLoadIdentity();
	}
    if (nivel == 4) {
    	enemigo(-280.0f, 250.0f, 0.4f, 0.6f, 0.4f, 0.2f, 0.02f);
    	glLoadIdentity();
    	enemigo(100.0f, 250.0f, 0.4f, 0.6f, 0.4f, 0.2f,0.02f);
    	glLoadIdentity();
	}
    if(mostrar){
    	Link();
	}
    glLoadIdentity();

    glutSwapBuffers();
}

void Teclado(int key, int x, int y) {
    // Manejar los eventos de teclado
    switch (key) {
        case GLUT_KEY_UP:// Mover hacia arriba
        if(posY == 410.0f && posX>=100.0f){
        	posX=0.0f;
        	posY=-550.0f;
    		axu=Nivel2;
    		nivel = 2;
		}
		
		if(posY>=600.0f){
			posX=0.0f;
			posY=500.0f;
			axu=Nivel;
			nivel = 1;
		}
		if(posX <=20.f && posX >= -20.0f && posY >= -80.0f && nivel == 2){
			axu=Nivel2;
			nivel = 5;
			esp=true;
		}
            posY += 10.0f; 
            break;
        case GLUT_KEY_DOWN:// Mover hacia abajo
        	if(posY<=-590.0f){
        		posX=250.0f;
        		posY=410.0f;
        		axu=Nivel;
        		nivel = 1;
			}
            posY -= 10.0f; 
            break;
        case GLUT_KEY_LEFT: // Mover hacia la izquierda
        	if(posX<=-600.0f && posY<=-280.0f && posY>=-420.0f){
        		posX=580.0f;
        		posY=0.0;
        		axu=Nivel4;
        		nivel = 4;
			}
			if(posX<=-600.0f && posY<=300.0f && posY>=-60.0f){
				posX=-550.0f;
				posY=-40.0f;
				axu=ini;
				mostrar = false;
				nivel = 0;
			}
			if(posX >= 0.0f && posX<=50.0f && posY>=-280.0f && posY<=200.0f && nivel == 3){
				vidatotal--;
				posX = -550.0f;
				posY= -40.0f;
				axu=Nivel;
				nivel = 1;
				esp=false;
			}
			if(posX <= -480.0f && posX>=-600.0f && posY>=-280.0f && posY<=250.0f && nivel == 3){
				vidatotal--;
				posX = -550.0f;
				posY= -40.0f;
				axu=Nivel;
				nivel = 1;
				esp=false;
			}
            posX -= 10.0f;
            break;
        case GLUT_KEY_RIGHT:// Mover hacia la derecha
        	if(posX >= 600.0f && posY <= 150.0f && posY>=-150.0f){
        		posX=550.0f;
        		posY=0.0f;
    			axu=Nivel3;
    			nivel = 3;
			}
            posX += 10.0f; 
            break;
    }

    // Solicitar que se redibuje la escena
    glutPostRedisplay();
}

void tecladoNormal(unsigned char key, int x, int y){
	switch (key) {
        case 13: 
			posX = -550.0f;
			posY = -40.0f;
    		axu=Nivel;
    		nivel = 1;
    		mostrar = true;
    		vidatotal=3;
        break;
        
        case 32:
        	axu=ini;
        	nivel = 0;
        	vidatotal = 3;
    }
    glutPostRedisplay();
}

void reshape(int largo, int ancho){
	glViewport(0,0,(GLsizei)largo,(GLsizei)ancho);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	int t=600;
	glOrtho(-t,t,-t,t,-t,t);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void idle(){
	angulo+=0.01;
	display();
}

int main(int argc, char** argv) {
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGBA |GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(600,600);
	glutInitWindowPosition(100,100);
	glutCreateWindow("Legend of Zelda");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(Teclado);
	glutKeyboardFunc(tecladoNormal);
	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}
