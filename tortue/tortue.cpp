#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <cstdlib>
#include <iostream>

//!
//! \brief : Classe point
//! \details : Permet de définir un point avec son code RGB
//!
//! \author : S.Lanquetin
//! \source : cube.cpp | TP 0 - Synthèse d'image
//!
class Point{
    public :
        double x;
        double y;
        double z;
        float r;
        float g;
        float b;
};

char presse;
int anglex,angley,x,y,xold,yold;

/* Prototype des fonctions */
void affichage();
void clavier(unsigned char touche,int x,int y);
void reshape(int x,int y);
void idle();
void mouse(int bouton,int etat,int x,int y);
void mousemotion(int x,int y);
void handleArrowKeys(int key, int x, int y);
void handleArrowkeysReleased(int key, int x, int y);

// Fonction de dessin
void drawBody();
void drawShell();
void drawUnder();
void drawFeets();
void drawHead();
void drawTail();
void drawEyes();
void drawPupil();

// animation
void animHeadAndTail();
int angle=-30;
bool left=true;

bool feetsReverse=false;
GLfloat moveFeets=0.35;
void animFeets();

// Zoom et mouvements
float zoomValue = 1;

bool rightKeyPressed = false;
bool leftKeyPressed = false;
bool upKeyPressed = false;
bool downKeyPressed = false;
int verticalMove = 0;
int horizontalMove = 0;

// lumières
void initLightning();

//!
//! \brief : Fonction main
//! \details : Permet d'initialiser OpenGL
//!
//! \authors : S.Lanquetin | V.Marguerie
//!
int main(int argc,char **argv)
{
    /* initialisation de glut et creation de la fenetre */
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(200,200);
    glutInitWindowSize(500,500);
    glutCreateWindow("Tortue");

    /* Initialisation d'OpenGL */
    glClearColor(0.0,0.0,0.0,0.0);
    glColor3f(1.0,1.0,1.0);
    glPointSize(2.0);
    glEnable(GL_DEPTH_TEST);
    glutIdleFunc(animHeadAndTail);

    /* enregistrement des fonctions de rappel */
    glutDisplayFunc(affichage);
    glutKeyboardFunc(clavier);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(mousemotion);
    glutSpecialFunc(handleArrowKeys);
    glutSpecialUpFunc(handleArrowkeysReleased);

    /* Entree dans la boucle principale glut */
    glutMainLoop();
    return 0;
}


//!
//! \brief : Fonction affichage
//! \details : Permet d'afficher les formes et les axes
//!
//! \authors : V.Marguerie | S.Lanquetin
//!
void affichage()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    initLightning();

    // Mise en place du zoom
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Zoom
    glOrtho(-zoomValue,zoomValue,-zoomValue,zoomValue,-zoomValue,zoomValue);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    // Gestion mouvements flêches
    glRotatef(verticalMove, 1.0, 0.0, 0.0);
    glRotatef(horizontalMove, 0.0, 1.0, 0.0);
    glRotatef(angley,1.0,0.0,0.0);
    glRotatef(anglex,0.0,1.0,0.0);

    drawBody();
    drawShell();
    drawUnder();
    drawFeets();

    glPushMatrix();
        glRotated(angle/3, 0, 1, 0);
        drawTail();
    glPopMatrix();

    glPushMatrix();
        glRotated(-angle, 1, 0, 0);
        drawHead();
        drawEyes();

        glPushMatrix();
        glRotated(-angle/15, 1, 1, 1);
            drawPupil();
        glPopMatrix();
    glPopMatrix();

    //Repère
    //axe x en rouge
    glBegin(GL_LINES);
        glColor3f(1.0,0.0,0.0);
    	glVertex3f(0, 0,0.0);
    	glVertex3f(1, 0,0.0);
    glEnd();

    //axe des y en vert
    glBegin(GL_LINES);
    	glColor3f(0.0,1.0,0.0);
    	glVertex3f(0, 0,0.0);
    	glVertex3f(0, 1,0.0);
    glEnd();

    //axe des z en bleu
    glBegin(GL_LINES);
    	glColor3f(0.0,0.0,1.0);
    	glVertex3f(0, 0,0.0);
    	glVertex3f(0, 0,1.0);
    glEnd();

    glFlush();

    //On echange les buffers
    glutSwapBuffers();
}

//!
//! \brief : fonction initLightning
//! \details : Permet de définir les lumières
//!
//! Construction de la lumìere en deux points,
//! une lumière principale de couleur rouge donnant la teinte
//! et une seconde (dite low-key) en support pour donner de
//! la profondeur à l'éclairage et nuancer fortement les ombres.
//!
//! \author : V.Marguerie
//!
void initLightning()
{
    // front light
    GLfloat lightColor0[] = {1.0f, 0.0f, 0.0f, 1.0f};
    // Gestion position
    GLfloat lightPos0[] = {2.0f, 0.5f, 2.0f, 0.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

    // low-key light
    GLfloat lightColor1[] = {0.6f, 0.6f, 0.6f, 1.0f};
    // Gestion position
    GLfloat lightPos1[] = {2.0f, -1.0f, 3.0f, 0.0f};
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);


    /* Initialisation pour la lumière */
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
}


//!
//! \brief : fonction clavier
//! \details : Permet de définir définir des actions au clavier
//!
//! \author : S.Lanquetin | V.Marguerie
//! \source : cube.cpp | TP 0 - Synthèse d'image
//!
void clavier(unsigned char touche,int x,int y)
{
    switch (touche)
    {
        case 'p':
            glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
            glutPostRedisplay();
        break;
        case 'f':
            glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
            glutPostRedisplay();
        break;
        case 's' :
            glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
            glutPostRedisplay();
        break;
        case 'd':
            glEnable(GL_DEPTH_TEST);
            glutPostRedisplay();
        break;
        case 'D':
            glDisable(GL_DEPTH_TEST);
            glutPostRedisplay();
        break;
        case 'a':
            glPolygonMode(GL_FRONT,GL_FILL);
            glPolygonMode(GL_FRONT,GL_LINE);
            glutPostRedisplay();
        break;
        case 'z': {
            if(zoomValue == 1) break;
            zoomValue -= 0.25;
        }
        break;
        case 'Z': {
            if(zoomValue == 15) break;
            zoomValue += 0.25;
        }
        break;
        case 'm':
            animFeets();
        break;
        case 'q' :
            exit(0);
    }
}


//!
//! \brief : fonction handleArrowKeys
//! \details : Permet de définir définir les actions des flèches
//!
//! \author : V.Marguerie
//!
void handleArrowKeys(int key, int x, int y)
{
    switch (key)
    {
        case GLUT_KEY_LEFT:
            leftKeyPressed = true;
            if(leftKeyPressed) horizontalMove -= 2;
        break;
        case GLUT_KEY_RIGHT:
            rightKeyPressed = true;
            if(rightKeyPressed) horizontalMove += 2;
        break;
        case GLUT_KEY_UP:
            upKeyPressed = true;
            if(upKeyPressed) verticalMove -= 2;
        break;
        case GLUT_KEY_DOWN:
            downKeyPressed = true;
            if(downKeyPressed) verticalMove += 2;
        break;
    }
}


//!
//! \brief : fonction handleArrowkeysReleased
//! \details : Permet de gérer le relachement des flèches
//!
//! \author : V.Marguerie
//!
void handleArrowkeysReleased(int key, int x, int y)
{
    switch (key)
    {
        case GLUT_KEY_LEFT:
            leftKeyPressed = false;
        break;
        case GLUT_KEY_RIGHT:
            rightKeyPressed = false;
        break;
        case GLUT_KEY_UP:
            upKeyPressed = false;
        break;
        case GLUT_KEY_DOWN:
            downKeyPressed = false;
        break;
    }
}

//!
//! \brief : Fonction reshape
//! \details : Permet de conserver l'echelle des formes lors de changements de tailles de fenêtre
//!
//! \author : S.Lanquetin
//! \source : cube.cpp | TP 0 - Synthèse d'image
//!
void reshape(int x,int y)
{
  if (x<y)
    glViewport(0,(y-x)/2,x,x);
  else
    glViewport((x-y)/2,0,y,y);
}



//!
//! \brief : Fonction mouse
//! \details : Permet de définir des actions à la souris
//!
//! \author : S.Lanquetin
//! \source : cube.cpp | TP 0 - Synthèse d'image
//!
void mouse(int button, int state,int x,int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        presse = 1;
        xold = x;
        yold=y;
    }
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
        presse=0;
}


//!
//! \brief : Classe pointfalse
//! \details : Permet de définir les mouvements de la souris
//!
//! \author : S.Lanquetin
//! \source : cube.cpp | TP 0 - Synthèse d'image
//!
void mousemotion(int x,int y)
{
    if (presse)
    {
        anglex=anglex+(x-xold);
        angley=angley+(y-yold);
        glutPostRedisplay();
    }

    xold=x;
    yold=y;
}

//!
//! \brief : Function drawBody
//! \details : Permet de créer le corps
//!
//! \author : V.Marguerie
//!
void drawBody()
{
    glPushMatrix();
    	glColor3f(0.1,0.9,0.3);
        glScalef(0.6,0.2,0.5);
        glutSolidSphere(1,25,25);
    glPopMatrix();
}

//!
//! \brief : Function drawShell
//! \details : Permet de créer la carapace
//!
//! \author : V.Marguerie
//!
void drawShell()
{
    glPushMatrix();
    	glColor3f(0.7,0.5,0.1);
        glScalef(0.8,0.2,0.6);
        glTranslatef(0,0.5,0);
        glutSolidSphere(1,25,25);
    glPopMatrix();
}

//!
//! \brief : Function drawFeets
//! \details : Permet de créer les pieds
//!
//! \author : V.Marguerie
//!
void drawFeets()
{
    glColor3f(0.1,0.9,0.3);

    // Pied droit
    glPushMatrix();
        glTranslatef(moveFeets,-0.11,-0.35);
        glutSolidSphere(0.1,125,135);
    glPopMatrix();

    // Pied  gauche
    glPushMatrix();
        glTranslatef(moveFeets,-0.11,0.35);
        glutSolidSphere(0.1,125,135);
    glPopMatrix();

    // Pied arr droit
    glPushMatrix();
        glTranslatef(-moveFeets,-0.11,-0.35);
        glutSolidSphere(0.1,125,135);
    glPopMatrix();

    // Pied arr gauche
    glPushMatrix();
        glTranslatef(-moveFeets,-0.11,0.35);
        glutSolidSphere(0.1,125,135);
    glPopMatrix();
}

//!
//! \brief : Function drawHead
//! \details : Permet de créer la base de la tête
//!
//! \author : V.Marguerie
//!
void drawHead()
{
    glColor3f(0.1,0.9,0.3);

    glPushMatrix();
        glScalef(1.4,0.9,1.6);
        glTranslatef(0.6,0.15,0);
        glutSolidSphere(0.1,125,125);
    glPopMatrix();
}

//!
//! \brief : Function drawTail
//! \details : Permet de créer la queue
//!
//! \author : V.Marguerie
//!
void drawTail()
{
    glColor3f(0.1,0.9,0.3);

    glPushMatrix();
        glScalef(1.8,0.4,1);
        glTranslatef(-0.4,0,0);
        glutSolidSphere(0.1,125,125);
    glPopMatrix();
}

//!
//! \brief : Function drawUnder
//! \details : Permet de créer le dessous de la tortue
//!
//! \author : V.Marguerie
//!
void drawUnder()
{
    glColor3f(0.7,0.5,0.1);

    glPushMatrix();
        glScalef(0.6,0.2,0.4);
        glTranslatef(0,-0.1,0);
        glutSolidSphere(1,25,25);
    glPopMatrix();
}

//!
//! \brief : Function drawEyes
//! \details : Permet de créer les yeux de la tortue
//!
//! \author : V.Marguerie
//!
void drawEyes()
{
    glColor3f(1,1,1);

    // Droit
    glPushMatrix();
        glScalef(0.4,0.4,0.4);
        glTranslatef(2.34,0.48,-0.25);
        glutSolidSphere(0.1,125,125);
    glPopMatrix();

    // Gauche
    glPushMatrix();
        glScalef(0.4,0.4,0.4);
        glTranslatef(2.34,0.48,0.25);
        glutSolidSphere(0.1,125,125);
    glPopMatrix();
}

void drawPupil()
{
    glColor3f(0,0,0);

    // Pupille Droit
    glPushMatrix();
        glScalef(0.1,0.1,0.1);
        glTranslatef(9.7,2.05,-1.15);
        glutSolidSphere(0.1,125,125);
    glPopMatrix();

    // Pupille Gauche
    glPushMatrix();
        glScalef(0.1,0.1,0.1);
        glTranslatef(9.7,2.05,1.15);
        glutSolidSphere(0.1,125,125);
    glPopMatrix();
}

//!
//! \brief : Function animHeadAndTail
//! \details : Permet de créer l'animaton de la tête et de la queue
//!
//! \author : V.Marguerie
//!
void animHeadAndTail()
{
    left ? angle+=1 : angle-=1;

    if(angle>30) left=false;
    else if(angle<-30) left=true;

    glutPostRedisplay();
}

//!
//! \brief : Function animFeets
//! \details : Permet de créer l'animaton des pieds
//!
//! \author : V.Marguerie
//!
void animFeets()
{
    if(!feetsReverse) {
        moveFeets+=0.02;

        if(moveFeets > 0.5) feetsReverse=true;
    } else {
        moveFeets-=0.02;

        if(moveFeets < 0.25) feetsReverse=false;
    }
}
