#include "Astroid.h"
#include "Player.h"
#include "Projectile.h"
#include "Engine.h"

void Astroid::Update(float dt)
{
	Actor::Update(dt);

	if (scene->GetActor<Player>())
	{
		transform.position += PhoenixEngine::Vector2::Rotate(PhoenixEngine::Vector2::right, transform.rotation) * speed * dt;
		transform.position.x = PhoenixEngine::Wrap(transform.position.x, 0.0f, 800.0f);
		transform.position.y = PhoenixEngine::Wrap(transform.position.y, 0.0f, 600.0f);
	}
}

void Astroid::OnCollision(Actor* actor)
{
	if (dynamic_cast<Projectile*>(actor) && actor->tag == "Player")
	{
		actor->destroy = true;
		destroy = true;
		scene->engine->Get<PhoenixEngine::ParticleSystem>()->Create(transform.position, 200, 3, PhoenixEngine::Color::white, 250);
		scene->engine->Get<PhoenixEngine::AudioSystem>()->PlayAudio("explosion");

		PhoenixEngine::Event event;
		event.name = "AddPoints";
		event.data = 100;
		scene->engine->Get<PhoenixEngine::EventSystem>()->Notify(event);
	}
}
