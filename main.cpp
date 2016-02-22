#include "camera.h"
#include "Handle.h"
#include "wall.h"
#include "player.h"
#include "textures.h"

string normal[] = { "behind.bmp", "front.bmp", "front.bmp", "front.bmp", "front.bmp", "front.bmp" };
string ground[] = { "behind.bmp", "front.bmp", "front.bmp", "front.bmp", "front.bmp", "ground.bmp" };

GLuint* GetTextIds(string texture_names[6])
{
	GLuint *arr = new GLuint[6];
	for (int i = 0; i < 6; i++)
	{
		arr[i] = LoadTexture(texture_names[i].c_str());
	}
	return arr;
}

Wall * SkyBox;
bool pressed[4] = { false };
bool enemypressed[4] = { false };
#define TORQUE_POWER 100.0f
#define MOVE_POWER 40.0f
#define WALL_WHIGHT 100.0f
#define ROUND_POWER TORQUE_POWER
#define SFC_HANDLE 0.2f
#define SFC_PLAYER 0.15f
#define SFC_BALL 0.2f
#define SFC_GROUND 0.7f
#define SFC_WALLS 0.64f
#define RC_HANDEL 0.1f
#define RC_PLAYER 0.7f
#define RC_BALL 0.1f
#define RC_GROUND 0.2f
#define RC_WALL 0.3f

#define ROFC_HANDLE 0.2f
#define ROFC_PLAYER 0.2f
#define ROFC_BALL 0.2f
#define ROFC_GROUND 0.5f
#define ROFC_WALLS 0.3f
#define BALL_MASS 0.1f

bool Keys[256] = { false };
Vector3 x;
Camera camera(Vector3(StartPoint.X() + 50, StartPoint.Y() + 90, StartPoint.Z()));
Ball *ball;
int i = 1;
Player *player;
Wall *Ground;
Wall *Left_side;
Wall *Forward_side_1;
Wall *Forward_side_2;
Wall *Up_forward;
Wall *Backward_side_1;
Wall *Backward_side_2;
Wall* Up_backward;
Wall *Right_side;

Wall *FeetGround;
Wall *first_leg;
Wall *second_leg;
Wall *third_leg;
Wall *forth_leg;

Handle *goalkeepers;
Handle *defensers;
Handle *miders;
Handle *attackers;
Handle *enemygoalkeepers;
Handle *enemydefensers;
Handle *enemymiders;
Handle *enemyattackers;
GLuint* texture_ids;

Wall *MyGoal;
Wall *EnemyGoal;

Handle *Hand;
int elapsedtime = 0;
float handleradius = 0.4f;
float deltatime;
float oldtime;
float newtime;
const Vector3 handcolor(1, 0, 0);
const Vector3 playercolor(0, 0, 1);
const Vector3 enemyplayercolor(1, 0, 0);
Force ballweight(Vector3(0, -1, 0), BALL_MASS*gravity);
Force ballforce;
Force handleforces[8];
/* GLUT callback Handlers */
void resize(int width, int height)
{
	const float ar = (float)width / (float)height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-ar, ar, -1.0, 1.0, 2, 2000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camera.Position.X(), camera.Position.Y(), camera.Position.Z(), StartPoint.X(), StartPoint.Y(), StartPoint.Z(), 0, 1, 0);
}
void Keyboard_Press();
void initialize_handleforces()
{
	for (int i = 0; i < 8; i++)
		handleforces[i] = Force(Vector3(0, 0, 0), 0);
}

void handlephysics()
{
	ballforce = ballweight;
	initialize_handleforces();
	Keyboard_Press();
	Ground->collidewithball(*ball, ballforce);
	Backward_side_1->collidewithball(*ball, ballforce);
	Backward_side_2->collidewithball(*ball, ballforce);
	Forward_side_1->collidewithball(*ball, ballforce);
	Forward_side_2->collidewithball(*ball, ballforce);
	Left_side->collidewithball(*ball, ballforce);
	Right_side->collidewithball(*ball, ballforce);
	Up_forward->collidewithball(*ball, ballforce);
	Up_backward->collidewithball(*ball, ballforce);
	FeetGround->collidewithball(*ball, ballforce);
	goalkeepers->collidewithball(*ball, ballforce);
	defensers->collidewithball(*ball, ballforce);
	miders->collidewithball(*ball, ballforce);

	attackers->collidewithball(*ball, ballforce);
	enemygoalkeepers->collidewithball(*ball, ballforce);
	enemydefensers->collidewithball(*ball, ballforce);
	enemymiders->collidewithball(*ball, ballforce);
	enemyattackers->collidewithball(*ball, ballforce);

	goalkeepers->collidewallwithballs(*Right_side, handleforces[0]);
	defensers->collidewallwithballs(*Right_side, handleforces[1]);
	miders->collidewallwithballs(*Right_side, handleforces[2]);
	attackers->collidewallwithballs(*Right_side, handleforces[3]);
	enemygoalkeepers->collidewallwithballs(*Right_side, handleforces[4]);
	enemydefensers->collidewallwithballs(*Right_side, handleforces[5]);
	enemymiders->collidewallwithballs(*Right_side, handleforces[6]);
	enemyattackers->collidewallwithballs(*Right_side, handleforces[7]);

	goalkeepers->collidewallwithballs(*Left_side, handleforces[0]);
	defensers->collidewallwithballs(*Left_side, handleforces[1]);
	miders->collidewallwithballs(*Left_side, handleforces[2]);
	attackers->collidewallwithballs(*Left_side, handleforces[3]);
	enemygoalkeepers->collidewallwithballs(*Left_side, handleforces[4]);
	enemydefensers->collidewallwithballs(*Left_side, handleforces[5]);
	enemymiders->collidewallwithballs(*Left_side, handleforces[6]);
	enemyattackers->collidewallwithballs(*Left_side, handleforces[7]);

	goalkeepers->applyforce(handleforces[0], false);
	defensers->applyforce(handleforces[1], false);
	miders->applyforce(handleforces[2], false);
	attackers->applyforce(handleforces[3], false);
	enemygoalkeepers->applyforce(handleforces[4], false);
	enemydefensers->applyforce(handleforces[5], false);
	enemymiders->applyforce(handleforces[6], false);
	enemyattackers->applyforce(handleforces[7], false);

	ball->applyforce(ballforce, false);

	ball->proceedintime();
	miders->proceedintime();
	defensers->proceedintime();
	attackers->proceedintime();
	goalkeepers->proceedintime();

	enemydefensers->proceedintime();
	enemymiders->proceedintime();
	enemyattackers->proceedintime();
	enemygoalkeepers->proceedintime();

}
void KeyboardDown(unsigned char key, int x, int y)
{
	Keys[key] = true;
}
void KeyboardUp(unsigned char key, int x, int y)
{
	Keys[key] = false;
}
void keyboardSpecialUp(int k, int x, int y)
{
	Keys[k] = false;
}
void keyboardSpecialDown(int k, int x, int y)
{
	Keys[k] = true;
}
void Keyboard_Press(void)
{
	if (Keys['z'] || Keys['Z'])
	{
		//goal keepers
		if (pressed[3])
			pressed[3] = false;
		else
			pressed[3] = true;
		goalkeepers->stop(false);
	}
	if (Keys['x'] || Keys['X'])
	{
		//defencers
		if (pressed[2])
			pressed[2] = false;
		else
			pressed[2] = true;
		defensers->stop(false);



	}
	if (Keys['c'] || Keys['C'])
	{
		if (pressed[1])
			pressed[1] = false;
		else
			pressed[1] = true;
		miders->stop(false);
	}
	if (Keys['v'] || Keys['V'])
	{
		if (pressed[0])
			pressed[0] = false;
		else
			pressed[0] = true;
		goalkeepers->stop(false);

	}
	if (Keys['r'] || Keys['R'])
	{
		//apply torque
		if (pressed[0])
		{
			attackers->applytorque(-Vector3((attackers->getmass() + attackers->getplayer(0)->getmass() * 3 + attackers->getball(0)->getmass() * 2)*TORQUE_POWER, 0, 0), false);

			//attackers
		}
		if (pressed[1])
		{
			miders->applytorque(-Vector3((miders->getmass() + miders->getplayer(0)->getmass() * 5 + miders->getball(0)->getmass() * 2) * TORQUE_POWER, 0, 0), false);

			//mider
		}
		if (pressed[2])
		{
			//dif
			defensers->applytorque(-Vector3((defensers->getmass() + defensers->getplayer(0)->getmass() * 2 + defensers->getball(0)->getmass() * 2) * TORQUE_POWER, 0, 0), false);

		}
		if (pressed[3])
		{
			//goal kepper
			goalkeepers->applytorque(-Vector3((goalkeepers->getmass() + goalkeepers->getplayer(0)->getmass() + goalkeepers->getball(0)->getmass() * 2) * TORQUE_POWER, 0, 0), false);

		}
	}


	if (Keys['m'] || Keys['M'])//0
	{
		if (enemypressed[0])
			enemypressed[0] = false;
		else
			enemypressed[0] = true;//attack
		enemyattackers->stop(false);
	}
	if (Keys[','] || Keys['<'])//1
	{
		if (enemypressed[1])
			enemypressed[1] = false;
		else
			enemypressed[1] = true;//midder
		enemymiders->stop(false);
	}
	if (Keys['.'] || Keys['>'])
	{
		if (enemypressed[2])
			enemypressed[2] = false;
		else
			enemypressed[2] = true;//dif
		enemydefensers->stop(false);

	}
	if (Keys['/'] || Keys['?'])
	{
		if (enemypressed[3])
			enemypressed[3] = false;
		else
			enemypressed[3] = true;//goalkepper
		enemygoalkeepers->stop(false);
	}
	if (Keys['[']||Keys['{'])//0
	{

		//apply torque
		if (enemypressed[0])
		{
			enemyattackers->applytorque(Vector3((enemyattackers->getmass() + enemyattackers->getplayer(0)->getmass() * 3 + enemyattackers->getball(0)->getmass() * 2)*TORQUE_POWER, 0, 0), false);

			//e attackers
		}
		if (enemypressed[1])
		{
			enemymiders->applytorque(Vector3((enemymiders->getmass() + enemymiders->getplayer(0)->getmass() * 5 + enemymiders->getball(0)->getmass() * 2) * TORQUE_POWER, 0, 0), false);

			//e midders
		}
		if (enemypressed[2])
		{
			enemydefensers->applytorque(Vector3((enemydefensers->getmass() + enemydefensers->getplayer(0)->getmass() * 2 + enemydefensers->getball(0)->getmass() * 2) * TORQUE_POWER, 0, 0), false);

			//e deffensers
		}
		if (enemypressed[3])
		{
			enemygoalkeepers->applytorque(Vector3((enemygoalkeepers->getmass() + enemygoalkeepers->getplayer(0)->getmass() + enemygoalkeepers->getball(0)->getmass() * 2) * TORQUE_POWER, 0, 0), false);

			//e goal keppers 
		}
	}

	if (Keys[']']||Keys['}'])
	{

		//apply force
		if (enemypressed[3])
		{

			handleforces[4] += Force(Vector3(-1, 0, 0), MOVE_POWER);

		}
		if (enemypressed[2])
		{
			handleforces[5] += Force(Vector3(-1, 0, 0), MOVE_POWER);

		}
		if (enemypressed[1])
		{
			handleforces[6] += Force(Vector3(-1, 0, 0), MOVE_POWER);

		}
		if (enemypressed[0])
		{
			handleforces[7] += Force(Vector3(-1, 0, 0), MOVE_POWER);

		}

	}
	if (Keys['\'']||Keys['\"'])
	{

		//apply negative force

		//apply force
		if (enemypressed[3])
		{
			handleforces[4] -= Force(Vector3(-1, 0, 0), MOVE_POWER);

		}
		if (enemypressed[2])
		{
			handleforces[5] -= Force(Vector3(-1, 0, 0), MOVE_POWER);

		}
		if (enemypressed[1])
		{
			handleforces[6] -= Force(Vector3(-1, 0, 0), MOVE_POWER);

		}
		if (enemypressed[0])
		{
			handleforces[7] -= Force(Vector3(-1, 0, 0), MOVE_POWER);

		}

	}
	if (Keys['T'] || Keys['t'])//8
	{

		//enemy apply force
		if (pressed[3])
		{
			//goal kepper
			handleforces[0] += Force(Vector3(-1, 0, 0), MOVE_POWER);

		}
		if (pressed[2])
		{
			//dif
			handleforces[1] += Force(Vector3(-1, 0, 0), MOVE_POWER);

		}
		if (pressed[1])
		{
			handleforces[2] += Force(Vector3(-1, 0, 0), MOVE_POWER);

			//mider
		}
		if (pressed[0])
		{
			handleforces[3] += Force(Vector3(-1, 0, 0), MOVE_POWER);
			//attackers
		}



	}
	if (Keys['g'] || Keys['G'])//5
	{
		if (pressed[3])
		{
			//goal kepper
			handleforces[0] -= Force(Vector3(-1, 0, 0), MOVE_POWER);

		}
		if (pressed[2])
		{
			//dif
			handleforces[1] -= Force(Vector3(-1, 0, 0), MOVE_POWER);

		}
		if (pressed[1])
		{
			handleforces[2] -= Force(Vector3(-1, 0, 0), MOVE_POWER);

			//mider
		}
		if (pressed[0])
		{
			handleforces[3] -= Force(Vector3(-1, 0, 0), MOVE_POWER);
			//attackers
		}
	}


	if (Keys['a'] || Keys['A'])//a
	{
		camera.MoveRight( -1 );
	}

	if (Keys['d'] || Keys['D'])//d
	{
		camera.MoveRight(1 );
	}

	if (Keys['w'] || Keys['W'])//w
	{
		camera.MoveForward(-2);
	}
	if (Keys['s'] || Keys['S'])//s
	{
		camera.MoveForward(2);
	}

	//if (Keys['e'] || Keys['E'])	 //e
	//{
	//camera.RotateX(10.0);

	//	}
	//if (Keys['q'] || Keys['Q'])//q
	//{
	//camera.RotateX(-10.0);

	//}
	//	if (Keys['f'] || Keys['F'])	 //f
	//{
	//	camera.Move(Vector3(0.0, -1.3f, 0.0));

	//	}
	//if (Keys['r'] || Keys['R'])//r
	//{
	//	camera.Move(Vector3(0.0, 1.3f, 0.0));

	//	}

	if (Keys[27])				 //esc
	{
		exit(0);

	}
	glutPostRedisplay();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	//gluLookAt(camera.Position.X(), camera.Position.Y(), camera.Position.Z(), ball->getcenterofmass().X(), ball->getcenterofmass().Y(), ball->getcenterofmass().Z(), 0, 1, 0);
	gluLookAt(camera.Position.X(), camera.Position.Y(), camera.Position.Z(), StartPoint.X(), StartPoint.Y(), StartPoint.Z(), 0, 1, 0);

	handlephysics();
	SkyBox->draw(Vector3(1, 1, 1));
	ball->draw(Vector3(1, 1, 1));
	Ground->draw(Vector3(1, 1, 1));
	FeetGround->draw(Vector3(1, 1, 1));
	Backward_side_1->draw(Vector3(1, 1, 1));
	Backward_side_2->draw(Vector3(1, 1, 1));
	Forward_side_1->draw(Vector3(1, 1, 1));
	Forward_side_2->draw(Vector3(1, 1, 1));
	Left_side->draw(Vector3(1, 1, 1));
	Right_side->draw(Vector3(1, 1, 1));
	Up_forward->draw(Vector3(1, 1, 1));
	Up_backward->draw(Vector3(1, 1, 1));
	first_leg->draw(Vector3(1, 1, 1));
	second_leg->draw(Vector3(1, 1, 1));
	third_leg->draw(Vector3(1, 1, 1));
	forth_leg->draw(Vector3(1, 1, 1));
	MyGoal->draw(Vector3(0, 0, 0));
	EnemyGoal->draw(Vector3(0, 0, 0));



	goalkeepers->draw(playercolor);
	defensers->draw(playercolor);
	miders->draw(playercolor);
	attackers->draw(playercolor);

	enemygoalkeepers->draw(enemyplayercolor);
	enemydefensers->draw(enemyplayercolor);
	enemymiders->draw(enemyplayercolor);
	enemyattackers->draw(enemyplayercolor);


	glutSwapBuffers();
}
void idle(void)
{
	glutPostRedisplay();
}
void init()
{
	glClearColor(1, 1, 1, 1);




	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_LIGHTING);

	glEnable(GL_LIGHT0);
	const GLfloat light_ambient[] = { 1.0f, 1.0f, 1.0f, 0.0f };
	const GLfloat light_diffuse[] = { 1.0f, 1, 1, 1.0f };
	const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	const GLfloat light_position[] = { 0.0f, 30, 0.0f, 1.0f };
	const GLfloat light_shininess[] = { 50.0f };
	const GLfloat light_emissive[] = { 1, -1, 1, 0 };

	glEnable(GL_COLOR_MATERIAL);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_EMISSION, light_emissive);

	glMaterialfv(GL_FRONT, GL_SPECULAR, light_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, light_shininess);
}

void fill_skybox()
{
	string textures[6] = { "Sup.bmp", "Sleft.bmp", "Sfront.bmp", "Sright.bmp", "Sback.bmp", "Sground.bmp" };
	SkyBox = new Wall(1000, 1000, 1000, WALL_WHIGHT * 100, StartPoint, Line(Vector3(), Vector3()), false, Vector3(), 0, GetTextIds(textures));
	FeetGround = new Wall(1000, 1, 1000, WALL_WHIGHT * 100, StartPoint - Vector3(0, LEG_HEIGHT + 0.5f, 0), Line(Vector3(), Vector3()), false, Vector3(), 0, GetTextIds(textures));
}
/* Program entry point */
int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(300, 200);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("FoosBall");
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Nice perspective corrections
	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutKeyboardFunc(KeyboardDown);
	glutKeyboardUpFunc(KeyboardUp);
	glutSpecialFunc(keyboardSpecialDown);
	glutSpecialUpFunc(keyboardSpecialUp);
	glutIdleFunc(idle);
	init();

	glutIgnoreKeyRepeat(true);

	texture_ids = GetTextIds(ground);

	Ground = new Wall(TABLE_WIDTH, 1, TABLE_DIPTH, WALL_WHIGHT * 100, Vector3(StartPoint.X(), StartPoint.Y(), StartPoint.Z()), Line(Vector3(), Vector3()), false, Vector3(), 0, texture_ids);

	texture_ids = GetTextIds(normal);

	Right_side = new Wall(WALL_THICKNESS, TABLE_HEIGHT, TABLE_DIPTH, WALL_WHIGHT, Vector3(StartPoint.X() + HALF_WIDTH, StartPoint.Y() + HALF_HEIGHT, StartPoint.Z()), Line(Vector3(), Vector3()), false, Vector3(), 0, texture_ids);
	Left_side = new Wall(WALL_THICKNESS, TABLE_HEIGHT, TABLE_DIPTH, WALL_WHIGHT, Vector3(StartPoint.X() - HALF_WIDTH, StartPoint.Y() + HALF_HEIGHT, StartPoint.Z()), Line(Vector3(), Vector3()), false, Vector3(), 0, texture_ids);
	Forward_side_1 = new Wall(BOARD_WIDTH - HALF_WALL_THICKNESS, TABLE_HEIGHT, WALL_THICKNESS, WALL_WHIGHT, Vector3(StartPoint.X() + HALF_WALL_THICKNESS - (BOARD_WIDTH + GOAL_WIDTH) / 2, StartPoint.Y() + HALF_HEIGHT, StartPoint.Z() + HALF_DIPTH - HALF_WALL_THICKNESS), Line(Vector3(), Vector3()), false, Vector3(), 0, texture_ids);
	Forward_side_2 = new Wall(BOARD_WIDTH - HALF_WALL_THICKNESS, TABLE_HEIGHT, WALL_THICKNESS, WALL_WHIGHT, Vector3(StartPoint.X() - HALF_WALL_THICKNESS + (BOARD_WIDTH + GOAL_WIDTH) / 2, StartPoint.Y() + HALF_HEIGHT, StartPoint.Z() + HALF_DIPTH - HALF_WALL_THICKNESS), Line(Vector3(), Vector3()), false, Vector3(), 0, texture_ids);
	MyGoal = new Wall(GOAL_WIDTH - HALF_WALL_THICKNESS, GOAL_HEIGHT, WALL_THICKNESS, 0, Vector3(StartPoint.X(), StartPoint.Y() + HALF_GOAL_HEIGHT, StartPoint.Z() + HALF_DIPTH - HALF_WALL_THICKNESS), Line(Vector3(), Vector3()), false, Vector3(), 0, NULL);
	Up_forward = new Wall(GOAL_WIDTH - HALF_WALL_THICKNESS, TABLE_HEIGHT - GOAL_HEIGHT, WALL_THICKNESS, WALL_WHIGHT, Vector3(StartPoint.X(), StartPoint.Y() + HALF_HEIGHT + HALF_GOAL_HEIGHT, StartPoint.Z() + HALF_DIPTH - HALF_WALL_THICKNESS), Line(Vector3(), Vector3()), false, Vector3(), 0, texture_ids);
	Backward_side_1 = new Wall(BOARD_WIDTH - HALF_WALL_THICKNESS, TABLE_HEIGHT, WALL_THICKNESS, WALL_WHIGHT, Vector3(StartPoint.X() + HALF_WALL_THICKNESS - (BOARD_WIDTH + GOAL_WIDTH) / 2, StartPoint.Y() + HALF_HEIGHT, StartPoint.Z() - HALF_DIPTH + HALF_WALL_THICKNESS), Line(Vector3(), Vector3()), false, Vector3(), 0, texture_ids);
	Backward_side_2 = new Wall(BOARD_WIDTH - HALF_WALL_THICKNESS, TABLE_HEIGHT, WALL_THICKNESS, WALL_WHIGHT, Vector3(StartPoint.X() - HALF_WALL_THICKNESS + (BOARD_WIDTH + GOAL_WIDTH) / 2, StartPoint.Y() + HALF_HEIGHT, StartPoint.Z() - HALF_DIPTH + HALF_WALL_THICKNESS), Line(Vector3(), Vector3()), false, Vector3(), 0, texture_ids);
	EnemyGoal = new Wall(GOAL_WIDTH - HALF_WALL_THICKNESS, GOAL_HEIGHT, WALL_THICKNESS, 0, Vector3(StartPoint.X(), StartPoint.Y() + HALF_GOAL_HEIGHT, StartPoint.Z() - HALF_DIPTH + HALF_WALL_THICKNESS), Line(Vector3(), Vector3()), false, Vector3(), 0, NULL);
	Up_backward = new Wall(GOAL_WIDTH - HALF_WALL_THICKNESS, TABLE_HEIGHT - GOAL_HEIGHT, WALL_THICKNESS, WALL_WHIGHT, Vector3(StartPoint.X(), StartPoint.Y() + HALF_HEIGHT + HALF_GOAL_HEIGHT, StartPoint.Z() - HALF_DIPTH + HALF_WALL_THICKNESS), Line(Vector3(), Vector3()), false, Vector3(), 0, texture_ids);

	fill_skybox();



	first_leg = new Wall(LEG_WIDTH, LEG_HEIGHT, LEG_DIPTH, WALL_WHIGHT, Right_side->getcenterofmass() + Vector3(-HALF_LEG_WIDTH, -HALF_HEIGHT - HALF_LEG_HEIGHT, HALF_DIPTH - HALF_LEG_DIPTH), Line(Vector3(), Vector3()), false, Vector3(), 0, texture_ids);
	second_leg = new Wall(LEG_WIDTH, LEG_HEIGHT, LEG_DIPTH, WALL_WHIGHT, Right_side->getcenterofmass() + Vector3(-HALF_LEG_WIDTH, -HALF_HEIGHT - HALF_LEG_HEIGHT, -HALF_DIPTH + HALF_LEG_DIPTH), Line(Vector3(), Vector3()), false, Vector3(), 0, texture_ids);
	third_leg = new Wall(LEG_WIDTH, LEG_HEIGHT, LEG_DIPTH, WALL_WHIGHT, Left_side->getcenterofmass() + Vector3(HALF_LEG_WIDTH, -HALF_HEIGHT - HALF_LEG_HEIGHT, HALF_DIPTH - HALF_LEG_DIPTH), Line(Vector3(), Vector3()), false, Vector3(), 0, texture_ids);
	forth_leg = new Wall(LEG_WIDTH, LEG_HEIGHT, LEG_DIPTH, WALL_WHIGHT, Left_side->getcenterofmass() + Vector3(HALF_LEG_WIDTH, -HALF_HEIGHT - HALF_LEG_HEIGHT, -HALF_DIPTH + HALF_LEG_DIPTH), Line(Vector3(), Vector3()), false, Vector3(), 0, texture_ids);


	goalkeepers = new Handle(Handle_Type::goalkeeper, 3.8, StartPoint + Vector3(0, HALF_HEIGHT + handleradius, 6.6f*TABLE_DIPTH / 16.0f), Line(StartPoint + Vector3(0, HALF_HEIGHT + handleradius, 6.6f*TABLE_DIPTH / 16.0f), StartPoint + Vector3(1, HALF_HEIGHT + handleradius, 6.6f*TABLE_DIPTH / 16.0f)), handleradius, false, Vector3(1, 0, 0), 0);
	enemygoalkeepers = new Handle(Handle_Type::goalkeeper, 3.8, StartPoint + Vector3(0, HALF_HEIGHT + handleradius, -6.6f*TABLE_DIPTH / 16.0f), Line(StartPoint + Vector3(0, HALF_HEIGHT + handleradius, -6.6f*TABLE_DIPTH / 16.0f), StartPoint + Vector3(1, HALF_HEIGHT + handleradius, -6.6f*TABLE_DIPTH / 16.0f)), handleradius, false, Vector3(1, 0, 0), 0);

	defensers = new Handle(Handle_Type::defenser, 5.4, StartPoint + Vector3(0, HALF_HEIGHT + handleradius, 5.0f*TABLE_DIPTH / 16.0f), Line(StartPoint + Vector3(0, HALF_HEIGHT + handleradius, 5.0f*TABLE_DIPTH / 16.0f), StartPoint + Vector3(1, HALF_HEIGHT + handleradius, 5.0f*TABLE_DIPTH / 16.0f)), handleradius, false, Vector3(1, 0, 0), 0);
	enemydefensers = new Handle(Handle_Type::defenser, 5.4, StartPoint + Vector3(0, HALF_HEIGHT + handleradius, -5.0f*TABLE_DIPTH / 16.0f), Line(StartPoint + Vector3(0, HALF_HEIGHT + handleradius, -5.0f*TABLE_DIPTH / 16.0f), StartPoint + Vector3(1, HALF_HEIGHT + handleradius, -5.0f*TABLE_DIPTH / 16.0f)), handleradius, false, Vector3(1, 0, 0), 0);

	attackers = new Handle(Handle_Type::attacker, 5.4, StartPoint + Vector3(0, HALF_HEIGHT + handleradius, -3.0f*TABLE_DIPTH / 16.0f), Line(StartPoint + Vector3(0, HALF_HEIGHT + handleradius, -3.0f*TABLE_DIPTH / 16.0f), StartPoint + Vector3(1, HALF_HEIGHT + handleradius, -3.0f*TABLE_DIPTH / 16.0f)), handleradius, false, Vector3(1, 0, 0), 0);
	enemyattackers = new Handle(Handle_Type::attacker, 5.4, StartPoint + Vector3(0, HALF_HEIGHT + handleradius, 3.0f*TABLE_DIPTH / 16.0f), Line(StartPoint + Vector3(0, HALF_HEIGHT + handleradius, 3.0f*TABLE_DIPTH / 16.0f), StartPoint + Vector3(1, HALF_HEIGHT + handleradius, 3.0f*TABLE_DIPTH / 16.0f)), handleradius, false, Vector3(1, 0, 0), 0);

	miders = new Handle(Handle_Type::mider, 7, StartPoint + Vector3(0, HALF_HEIGHT + handleradius, TABLE_DIPTH / 16.0f), Line(StartPoint + Vector3(0, HALF_HEIGHT + handleradius, TABLE_DIPTH / 16.0f), StartPoint + Vector3(1, HALF_HEIGHT + handleradius, TABLE_DIPTH / 16.0f)), handleradius, false, Vector3(1, 0, 0), 0);
	enemymiders = new Handle(Handle_Type::mider, 7, StartPoint + Vector3(0, HALF_HEIGHT + handleradius, -TABLE_DIPTH / 16.0f), Line(StartPoint + Vector3(0, HALF_HEIGHT + handleradius, -TABLE_DIPTH / 16.0f), StartPoint + Vector3(1, HALF_HEIGHT + handleradius, -TABLE_DIPTH / 16.0f)), handleradius, false, Vector3(1, 0, 0), 0);

	goalkeepers->fillconstants(Right_side);
	defensers->fillconstants(Right_side);
	attackers->fillconstants(Right_side);
	miders->fillconstants(Right_side);

	enemygoalkeepers->fillconstants(Right_side);
	enemydefensers->fillconstants(Right_side);
	enemyattackers->fillconstants(Right_side);
	enemymiders->fillconstants(Right_side);

	goalkeepers->fillconstants(Left_side);
	defensers->fillconstants(Left_side);
	attackers->fillconstants(Left_side);
	miders->fillconstants(Left_side);

	enemygoalkeepers->fillconstants(Left_side);
	enemydefensers->fillconstants(Left_side);
	enemyattackers->fillconstants(Left_side);
	enemymiders->fillconstants(Left_side);
    ball = new Ball(1.2, Vector3(1, 30, 9), BALL_MASS, Line(Vector3(0, 30, 9), Vector3(2, 30, 9)), true, Vector3(1, 1, 1), LoadTexture("ball.bmp"), 0);
	Body::StaticFrictionCoffeciants.insert(pair<BodyPair, float>(BodyPair(ball, goalkeepers), SFC_HANDLE));
	Body::StaticFrictionCoffeciants.insert(pair<BodyPair, float>(BodyPair(ball, enemygoalkeepers), SFC_HANDLE));
	Body::StaticFrictionCoffeciants.insert(pair<BodyPair, float>(BodyPair(ball, defensers), SFC_HANDLE));
	Body::StaticFrictionCoffeciants.insert(pair<BodyPair, float>(BodyPair(ball, enemydefensers), SFC_HANDLE));
	Body::StaticFrictionCoffeciants.insert(pair<BodyPair, float>(BodyPair(ball, attackers), SFC_HANDLE));
	Body::StaticFrictionCoffeciants.insert(pair<BodyPair, float>(BodyPair(ball, enemyattackers), SFC_HANDLE));
	Body::StaticFrictionCoffeciants.insert(pair<BodyPair, float>(BodyPair(ball, miders), SFC_HANDLE));
	Body::StaticFrictionCoffeciants.insert(pair<BodyPair, float>(BodyPair(ball, enemymiders), SFC_HANDLE));





	int n = goalkeepers->numberofplayers();
	for (int i = 0; i < n; i++)
	{
		Body::StaticFrictionCoffeciants.insert(pair<BodyPair, float>(BodyPair((ball), (goalkeepers->getplayer(i)->body)), SFC_PLAYER));

	}


	n = enemygoalkeepers->numberofplayers();
	for (int i = 0; i < n; i++)
	{
		Body::StaticFrictionCoffeciants.insert(pair<BodyPair, float>(BodyPair((ball), (enemygoalkeepers->getplayer(i)->body)), SFC_PLAYER));
	}

	n = defensers->numberofplayers();
	for (int i = 0; i < n; i++)
	{
		Body::StaticFrictionCoffeciants.insert(pair<BodyPair, float>(BodyPair((ball), (defensers->getplayer(i)->body)), SFC_PLAYER));
	}

	n = enemydefensers->numberofplayers();
	for (int i = 0; i < n; i++)
	{
		Body::StaticFrictionCoffeciants.insert(pair<BodyPair, float>(BodyPair((ball), (enemydefensers->getplayer(i)->body)), SFC_PLAYER));
	}

	n = attackers->numberofplayers();
	for (int i = 0; i < n; i++)
	{
		Body::StaticFrictionCoffeciants.insert(pair<BodyPair, float>(BodyPair((ball), (attackers->getplayer(i)->body)), SFC_PLAYER));
	}

	n = enemyattackers->numberofplayers();
	for (int i = 0; i < n; i++)
	{
		Body::StaticFrictionCoffeciants.insert(pair<BodyPair, float>(BodyPair((ball), (enemyattackers->getplayer(i)->body)), SFC_PLAYER));
	}

	n = miders->numberofplayers();
	for (int i = 0; i < n; i++)
	{
		Body::StaticFrictionCoffeciants.insert(pair<BodyPair, float>(BodyPair((ball), (miders->getplayer(i)->body)), SFC_PLAYER));
	}

	n = enemymiders->numberofplayers();
	for (int i = 0; i < n; i++)
	{
		Body::StaticFrictionCoffeciants.insert(pair<BodyPair, float>(BodyPair((ball), (enemymiders->getplayer(i)->body)), SFC_PLAYER));
	}



	n = goalkeepers->numberofballs();
	for (int i = 0; i < n; i++)
	{
		Body::StaticFrictionCoffeciants.insert(pair<BodyPair, float>(BodyPair((ball), (goalkeepers->getball(i))), SFC_BALL));
	}

	n = enemygoalkeepers->numberofballs();
	for (int i = 0; i < n; i++)
	{
		Body::StaticFrictionCoffeciants.insert(pair<BodyPair, float>(BodyPair((ball), (enemygoalkeepers->getball(i))), SFC_BALL));
	}

	n = defensers->numberofballs();
	for (int i = 0; i < n; i++)
	{
		Body::StaticFrictionCoffeciants.insert(pair<BodyPair, float>(BodyPair((ball), (defensers->getball(i))), SFC_BALL));
	}

	n = enemydefensers->numberofballs();
	for (int i = 0; i < n; i++)
	{
		Body::StaticFrictionCoffeciants.insert(pair<BodyPair, float>(BodyPair((ball), (enemydefensers->getball(i))), SFC_BALL));
	}

	n = attackers->numberofballs();
	for (int i = 0; i < n; i++)
	{
		Body::StaticFrictionCoffeciants.insert(pair<BodyPair, float>(BodyPair((ball), (attackers->getball(i))), SFC_BALL));
	}

	n = enemyattackers->numberofballs();
	for (int i = 0; i < n; i++)
	{
		Body::StaticFrictionCoffeciants.insert(pair<BodyPair, float>(BodyPair((ball), (enemyattackers->getball(i))), SFC_BALL));
	}

	n = miders->numberofballs();
	for (int i = 0; i < n; i++)
	{
		Body::StaticFrictionCoffeciants.insert(pair<BodyPair, float>(BodyPair((ball), (miders->getball(i))), SFC_BALL));
	}

	n = enemymiders->numberofballs();
	for (int i = 0; i < n; i++)
	{
		Body::StaticFrictionCoffeciants.insert(pair<BodyPair, float>(BodyPair((ball), (enemymiders->getball(i))), SFC_BALL));
	}


	Body::StaticFrictionCoffeciants.insert(pair<BodyPair, float>(BodyPair(ball, Ground), SFC_GROUND));


	Body::StaticFrictionCoffeciants.insert(pair<BodyPair, float>(BodyPair(ball, Forward_side_1), SFC_WALLS));
	Body::StaticFrictionCoffeciants.insert(pair<BodyPair, float>(BodyPair(ball, Forward_side_2), SFC_WALLS));
	Body::StaticFrictionCoffeciants.insert(pair<BodyPair, float>(BodyPair(ball, Backward_side_1), SFC_WALLS));
	Body::StaticFrictionCoffeciants.insert(pair<BodyPair, float>(BodyPair(ball, Backward_side_2), SFC_WALLS));
	Body::StaticFrictionCoffeciants.insert(pair<BodyPair, float>(BodyPair(ball, Right_side), SFC_WALLS));
	Body::StaticFrictionCoffeciants.insert(pair<BodyPair, float>(BodyPair(ball, Left_side), SFC_WALLS));
	Body::StaticFrictionCoffeciants.insert(pair<BodyPair, float>(BodyPair(ball, Up_forward), SFC_WALLS));
	Body::StaticFrictionCoffeciants.insert(pair<BodyPair, float>(BodyPair(ball, Up_backward), SFC_WALLS));




	Body::RestCoffeciants.insert(pair<BodyPair, float>(BodyPair(ball, goalkeepers), RC_HANDEL));
	Body::RestCoffeciants.insert(pair<BodyPair, float>(BodyPair(ball, enemygoalkeepers), RC_HANDEL));
	Body::RestCoffeciants.insert(pair<BodyPair, float>(BodyPair(ball, defensers), RC_HANDEL));
	Body::RestCoffeciants.insert(pair<BodyPair, float>(BodyPair(ball, enemydefensers), RC_HANDEL));
	Body::RestCoffeciants.insert(pair<BodyPair, float>(BodyPair(ball, attackers), RC_HANDEL));
	Body::RestCoffeciants.insert(pair<BodyPair, float>(BodyPair(ball, enemyattackers), RC_HANDEL));
	Body::RestCoffeciants.insert(pair<BodyPair, float>(BodyPair(ball, miders), RC_HANDEL));
	Body::RestCoffeciants.insert(pair<BodyPair, float>(BodyPair(ball, enemymiders), RC_HANDEL));


	n = goalkeepers->numberofplayers();
	for (int i = 0; i < n; i++)
	{
		Body::RestCoffeciants.insert(pair<BodyPair, float>(BodyPair(ball, goalkeepers->getplayer(i)->body), RC_PLAYER));
	}

	n = enemygoalkeepers->numberofplayers();
	for (int i = 0; i < n; i++)
	{
		Body::RestCoffeciants.insert(pair<BodyPair, float>(BodyPair(ball, enemygoalkeepers->getplayer(i)->body), RC_PLAYER));
	}

	n = defensers->numberofplayers();
	for (int i = 0; i < n; i++)
	{
		Body::RestCoffeciants.insert(pair<BodyPair, float>(BodyPair(ball, defensers->getplayer(i)->body), RC_PLAYER));
	}

	n = enemydefensers->numberofplayers();
	for (int i = 0; i < n; i++)
	{
		Body::RestCoffeciants.insert(pair<BodyPair, float>(BodyPair(ball, enemydefensers->getplayer(i)->body), RC_PLAYER));
	}

	n = attackers->numberofplayers();
	for (int i = 0; i < n; i++)
	{
		Body::RestCoffeciants.insert(pair<BodyPair, float>(BodyPair(ball, attackers->getplayer(i)->body), RC_PLAYER));
	}

	n = enemyattackers->numberofplayers();
	for (int i = 0; i < n; i++)
	{
		Body::RestCoffeciants.insert(pair<BodyPair, float>(BodyPair(ball, enemyattackers->getplayer(i)->body), RC_PLAYER));
	}

	n = miders->numberofplayers();
	for (int i = 0; i < n; i++)
	{
		Body::RestCoffeciants.insert(pair<BodyPair, float>(BodyPair(ball, miders->getplayer(i)->body), RC_PLAYER));
	}

	n = enemymiders->numberofplayers();
	for (int i = 0; i < n; i++)
	{
		Body::RestCoffeciants.insert(pair<BodyPair, float>(BodyPair(ball, enemymiders->getplayer(i)->body), RC_PLAYER));
	}


	n = goalkeepers->numberofballs();
	for (int i = 0; i < n; i++)
	{
		Body::RestCoffeciants.insert(pair<BodyPair, float>(BodyPair((ball), (goalkeepers->getball(i))), RC_BALL));
	}

	n = enemygoalkeepers->numberofballs();
	for (int i = 0; i < n; i++)
	{
		Body::RestCoffeciants.insert(pair<BodyPair, float>(BodyPair((ball), (enemygoalkeepers->getball(i))), RC_BALL));
	}

	n = defensers->numberofballs();
	for (int i = 0; i < n; i++)
	{
		Body::RestCoffeciants.insert(pair<BodyPair, float>(BodyPair((ball), (defensers->getball(i))), RC_BALL));
	}

	n = enemydefensers->numberofballs();
	for (int i = 0; i < n; i++)
	{
		Body::RestCoffeciants.insert(pair<BodyPair, float>(BodyPair((ball), (enemydefensers->getball(i))), RC_BALL));
	}

	n = attackers->numberofballs();
	for (int i = 0; i < n; i++)
	{
		Body::RestCoffeciants.insert(pair<BodyPair, float>(BodyPair((ball), (attackers->getball(i))), RC_BALL));
	}

	n = enemyattackers->numberofballs();
	for (int i = 0; i < n; i++)
	{
		Body::RestCoffeciants.insert(pair<BodyPair, float>(BodyPair((ball), (enemyattackers->getball(i))), RC_BALL));
	}

	n = miders->numberofballs();
	for (int i = 0; i < n; i++)
	{
		Body::RestCoffeciants.insert(pair<BodyPair, float>(BodyPair((ball), (miders->getball(i))), RC_BALL));
	}

	n = enemymiders->numberofballs();
	for (int i = 0; i < n; i++)
	{
		Body::RestCoffeciants.insert(pair<BodyPair, float>(BodyPair((ball), (enemymiders->getball(i))), RC_BALL));
	}


	Body::RestCoffeciants.insert(pair<BodyPair, float>(BodyPair(ball, Ground), RC_GROUND));


	Body::RestCoffeciants.insert(pair<BodyPair, float>(BodyPair(ball, Forward_side_1), RC_WALL));
	Body::RestCoffeciants.insert(pair<BodyPair, float>(BodyPair(ball, Forward_side_2), RC_WALL));
	Body::RestCoffeciants.insert(pair<BodyPair, float>(BodyPair(ball, Backward_side_1), RC_WALL));
	Body::RestCoffeciants.insert(pair<BodyPair, float>(BodyPair(ball, Backward_side_2), RC_WALL));
	Body::RestCoffeciants.insert(pair<BodyPair, float>(BodyPair(ball, Right_side), RC_WALL));
	Body::RestCoffeciants.insert(pair<BodyPair, float>(BodyPair(ball, Left_side), RC_WALL));
	Body::RestCoffeciants.insert(pair<BodyPair, float>(BodyPair(ball, Up_forward), RC_WALL));
	Body::RestCoffeciants.insert(pair<BodyPair, float>(BodyPair(ball, Up_backward), RC_WALL));





	Ball::RollingFrictionCoffs.insert(pair<BodyPair, float>(BodyPair(ball, goalkeepers), ROFC_HANDLE));
	Ball::RollingFrictionCoffs.insert(pair<BodyPair, float>(BodyPair(ball, enemygoalkeepers), ROFC_HANDLE));
	Ball::RollingFrictionCoffs.insert(pair<BodyPair, float>(BodyPair(ball, defensers), ROFC_HANDLE));
	Ball::RollingFrictionCoffs.insert(pair<BodyPair, float>(BodyPair(ball, enemydefensers), ROFC_HANDLE));
	Ball::RollingFrictionCoffs.insert(pair<BodyPair, float>(BodyPair(ball, attackers), ROFC_HANDLE));
	Ball::RollingFrictionCoffs.insert(pair<BodyPair, float>(BodyPair(ball, enemyattackers), ROFC_HANDLE));
	Ball::RollingFrictionCoffs.insert(pair<BodyPair, float>(BodyPair(ball, miders), ROFC_HANDLE));
	Ball::RollingFrictionCoffs.insert(pair<BodyPair, float>(BodyPair(ball, enemymiders), ROFC_HANDLE));


	n = goalkeepers->numberofplayers();
	for (int i = 0; i < n; i++)
	{
		Ball::RollingFrictionCoffs.insert(pair<BodyPair, float>(BodyPair(ball, goalkeepers->getplayer(i)->body), ROFC_PLAYER));
	}

	n = enemygoalkeepers->numberofplayers();
	for (int i = 0; i < n; i++)
	{
		Ball::RollingFrictionCoffs.insert(pair<BodyPair, float>(BodyPair(ball, enemygoalkeepers->getplayer(i)->body), ROFC_PLAYER));
	}

	n = defensers->numberofplayers();
	for (int i = 0; i < n; i++)
	{
		Ball::RollingFrictionCoffs.insert(pair<BodyPair, float>(BodyPair(ball, defensers->getplayer(i)->body), ROFC_PLAYER));
	}

	n = enemydefensers->numberofplayers();
	for (int i = 0; i < n; i++)
	{
		Ball::RollingFrictionCoffs.insert(pair<BodyPair, float>(BodyPair(ball, enemydefensers->getplayer(i)->body), ROFC_PLAYER));
	}

	n = attackers->numberofplayers();
	for (int i = 0; i < n; i++)
	{
		Ball::RollingFrictionCoffs.insert(pair<BodyPair, float>(BodyPair(ball, attackers->getplayer(i)->body), ROFC_PLAYER));
	}

	n = enemyattackers->numberofplayers();
	for (int i = 0; i < n; i++)
	{
		Ball::RollingFrictionCoffs.insert(pair<BodyPair, float>(BodyPair(ball, enemyattackers->getplayer(i)->body), ROFC_PLAYER));
	}

	n = miders->numberofplayers();
	for (int i = 0; i < n; i++)
	{
		Ball::RollingFrictionCoffs.insert(pair<BodyPair, float>(BodyPair(ball, miders->getplayer(i)->body), ROFC_PLAYER));
	}

	n = enemymiders->numberofplayers();
	for (int i = 0; i < n; i++)
	{
		Ball::RollingFrictionCoffs.insert(pair<BodyPair, float>(BodyPair(ball, enemymiders->getplayer(i)->body), ROFC_PLAYER));
	}

	n = goalkeepers->numberofballs();
	for (int i = 0; i < n; i++)
	{
		Ball::RollingFrictionCoffs.insert(pair<BodyPair, float>(BodyPair((ball), (goalkeepers->getball(i))), ROFC_BALL));
	}

	n = enemygoalkeepers->numberofballs();
	for (int i = 0; i < n; i++)
	{
		Ball::RollingFrictionCoffs.insert(pair<BodyPair, float>(BodyPair((ball), (enemygoalkeepers->getball(i))), ROFC_BALL));
	}

	n = defensers->numberofballs();
	for (int i = 0; i < n; i++)
	{
		Ball::RollingFrictionCoffs.insert(pair<BodyPair, float>(BodyPair((ball), (defensers->getball(i))), ROFC_BALL));
	}

	n = enemydefensers->numberofballs();
	for (int i = 0; i < n; i++)
	{
		Ball::RollingFrictionCoffs.insert(pair<BodyPair, float>(BodyPair((ball), (enemydefensers->getball(i))), ROFC_BALL));
	}

	n = attackers->numberofballs();
	for (int i = 0; i < n; i++)
	{
		Ball::RollingFrictionCoffs.insert(pair<BodyPair, float>(BodyPair((ball), (attackers->getball(i))), ROFC_BALL));
	}

	n = enemyattackers->numberofballs();
	for (int i = 0; i < n; i++)
	{
		Ball::RollingFrictionCoffs.insert(pair<BodyPair, float>(BodyPair((ball), (enemyattackers->getball(i))), ROFC_BALL));
	}

	n = miders->numberofballs();
	for (int i = 0; i < n; i++)
	{
		Ball::RollingFrictionCoffs.insert(pair<BodyPair, float>(BodyPair((ball), (miders->getball(i))), ROFC_BALL));
	}

	n = enemymiders->numberofballs();
	for (int i = 0; i < n; i++)
	{
		Ball::RollingFrictionCoffs.insert(pair<BodyPair, float>(BodyPair((ball), (enemymiders->getball(i))), ROFC_BALL));
	}



	Ball::RollingFrictionCoffs.insert(pair<BodyPair, float>(BodyPair(ball, Ground), ROFC_GROUND));



	Ball::RollingFrictionCoffs.insert(pair<BodyPair, float>(BodyPair(ball, Forward_side_1), ROFC_WALLS));
	Ball::RollingFrictionCoffs.insert(pair<BodyPair, float>(BodyPair(ball, Forward_side_2), ROFC_WALLS));
	Ball::RollingFrictionCoffs.insert(pair<BodyPair, float>(BodyPair(ball, Backward_side_1), ROFC_WALLS));
	Ball::RollingFrictionCoffs.insert(pair<BodyPair, float>(BodyPair(ball, Backward_side_2), ROFC_WALLS));
	Ball::RollingFrictionCoffs.insert(pair<BodyPair, float>(BodyPair(ball, Right_side), ROFC_WALLS));
	Ball::RollingFrictionCoffs.insert(pair<BodyPair, float>(BodyPair(ball, Left_side), ROFC_WALLS));
	Ball::RollingFrictionCoffs.insert(pair<BodyPair, float>(BodyPair(ball, Up_forward), ROFC_WALLS));
	Ball::RollingFrictionCoffs.insert(pair<BodyPair, float>(BodyPair(ball, Up_backward), ROFC_WALLS));


	glutMainLoop();
	return EXIT_SUCCESS;
}
