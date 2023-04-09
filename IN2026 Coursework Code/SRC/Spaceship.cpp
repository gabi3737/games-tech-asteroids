#include "GameUtil.h"
#include "GameWorld.h"
#include "Bullet.h"
#include "Spaceship.h"
#include "BoundingSphere.h"

using namespace std;

// PUBLIC INSTANCE CONSTRUCTORS ///////////////////////////////////////////////

/**  Default constructor. */
Spaceship::Spaceship()
	: GameObject("Spaceship"), mThrust(0)
{
}

/** Construct a spaceship with given position, velocity, acceleration, angle, and rotation. */
Spaceship::Spaceship(GLVector3f p, GLVector3f v, GLVector3f a, GLfloat h, GLfloat r)
	: GameObject("Spaceship", p, v, a, h, r), mThrust(0)
{
}

/** Copy constructor. */
Spaceship::Spaceship(const Spaceship& s)
	: GameObject(s), mThrust(0)
{
}

/** Destructor. */
Spaceship::~Spaceship(void)
{
}

// PUBLIC INSTANCE METHODS ////////////////////////////////////////////////////

/** Update this spaceship. */
void Spaceship::Update(int t)
{
	// Call parent update function
	GameObject::Update(t);
	if (powerUpTime >= 0)
	{
		powerUpTime -= t;
		poweredUp = true;
	}
	else
	{
		poweredUp = false;
	}
}

/** Render this spaceship. */
void Spaceship::Render(void)
{
	if (mSpaceshipShape.get() != NULL) mSpaceshipShape->Render();

	// If ship is thrusting
	if ((mThrust > 0) && (mThrusterShape.get() != NULL)) {
		mThrusterShape->Render();
	}

	GameObject::Render();
}

/** Fire the rockets. */
void Spaceship::Thrust(float t)
{
	mThrust = t;
	// Increase acceleration in the direction of ship
	mAcceleration.x = mThrust*cos(DEG2RAD*mAngle);
	mAcceleration.y = mThrust*sin(DEG2RAD*mAngle);
}

/** Set the rotation. */
void Spaceship::Rotate(float r)
{
	mRotation = r;
}

/** Shoot a bullet. */
void Spaceship::Shoot(void)
{
	// Check the world exists
	if (!mWorld) return;
	// Construct a unit length vector in the direction the spaceship is headed
	GLVector3f spaceship_heading(cos(DEG2RAD*mAngle), sin(DEG2RAD*mAngle), 0);
	spaceship_heading.normalize();
	// Calculate the point at the node of the spaceship from position and heading
	GLVector3f bullet_position = mPosition + (spaceship_heading * 4);
	// Calculate how fast the bullet should travel
	float bullet_speed = 30;
	// Construct a vector for the bullet's velocity
	GLVector3f bullet_velocity = mVelocity + spaceship_heading * bullet_speed;
	// Construct a new bullet
	shared_ptr<GameObject> bullet
		(new Bullet(bullet_position, bullet_velocity, mAcceleration, mAngle, 0, 2000));
	bullet->SetBoundingShape(make_shared<BoundingSphere>(bullet->GetThisPtr(), 2.0f));
	bullet->SetShape(mBulletShape);
	// Add the new bullet to the game world
	mWorld->AddObject(bullet);
	if (poweredUp)
	{
		// Construct a new bullet
		GLVector3f spaceship_heading(cos(DEG2RAD * mAngle+1), sin(DEG2RAD * mAngle+1), 0);
		spaceship_heading.normalize();
		// Calculate the point at the node of the spaceship from position and heading
		GLVector3f bullet_position = mPosition + (spaceship_heading * 4);
		// Calculate how fast the bullet should travel
		float bullet_speed = 30;
		// Construct a vector for the bullet's velocity
		GLVector3f bullet_velocity = mVelocity + spaceship_heading * bullet_speed;
		shared_ptr<GameObject> bullet2
		(new Bullet(bullet_position, bullet_velocity, mAcceleration, mAngle, 0, 2000));
		bullet2->SetBoundingShape(make_shared<BoundingSphere>(bullet2->GetThisPtr(), 2.0f));
		bullet2->SetShape(mBulletShape);
		// Add the new bullet to the game world
		mWorld->AddObject(bullet2);
		// Construct a new bullet
		GLVector3f spaceship_heading2(cos(DEG2RAD * mAngle - 1), sin(DEG2RAD * mAngle - 1), 0);
		spaceship_heading2.normalize();
		// Calculate the point at the node of the spaceship from position and heading
		GLVector3f bullet_position2 = mPosition + (spaceship_heading2 * 4);
		// Calculate how fast the bullet should travel
		float bullet_speed2 = 30;
		// Construct a vector for the bullet's velocity
		GLVector3f bullet_velocity2 = mVelocity + spaceship_heading2 * bullet_speed2;
		// Construct a new bullet
		shared_ptr<GameObject> bullet3
		(new Bullet(bullet_position2, bullet_velocity2, mAcceleration, mAngle, 0, 2000));
		bullet3->SetBoundingShape(make_shared<BoundingSphere>(bullet3->GetThisPtr(), 2.0f));
		bullet3->SetShape(mBulletShape);
		// Add the new bullet to the game world
		mWorld->AddObject(bullet3);
	}

}

bool Spaceship::CollisionTest(shared_ptr<GameObject> o)
{
	if (o->GetType() != GameObjectType("Asteroid")) return false;
	if (mBoundingShape.get() == NULL) return false;
	if (o->GetBoundingShape().get() == NULL) return false;
	return mBoundingShape->CollisionTest(o->GetBoundingShape());
}

void Spaceship::OnCollision(const GameObjectList &objects)
{
	mWorld->FlagForRemoval(GetThisPtr());
}