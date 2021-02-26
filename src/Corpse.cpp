#include "../include/Corpse.hpp"

namespace phy {

Corpse::Corpse(float mass, float elasticity, bool fixed, bool tied, bool etherial, sf::Color color) {
	static int global_corpse_id = 0;
	this->id = global_corpse_id++;

	this->fixed = fixed;
	this->tied = tied;
	this->etherial = etherial;
	this->removed = false;

	this->current_rotation = 0.0f;
	this->mass = mass;
	this->friction = 1.0f;

	this->color = color;
	set_elasticity(elasticity);
}

Corpse::~Corpse() = default;

int Corpse::get_id() const { return this->id; }
int Corpse::get_class() const { return ID_CORPSE;}

bool Corpse::get_fixed() const { return this->fixed; }
void Corpse::set_fixed(bool fixed) { this->fixed = fixed; }

bool Corpse::get_etherial() const { return this->etherial; }
void Corpse::set_etherial(bool etherial) { this->etherial = etherial; }

bool Corpse::get_removed() const { return this->removed; }
void Corpse::Remove() { this->removed = true; }

void Corpse::Step() {}
void Corpse::Stop() {}
void Corpse::Move(float x, float y, bool relative) {}
void Corpse::Move(sf::Vector2f move, bool relative) {}
void Corpse::Collision(std::shared_ptr<Corpse> a) {}
sf::Vector2f Corpse::get_diff_pos() const { return (this->current_pos - this->last_pos); }

void Corpse::CollisionResponse(phy::Corpse* corpse_a, phy::Corpse* corpse_b, const sf::Vector2f& force) {
	/* 
		/!\ This method only work for non rotating objects : 
		if an object is rotating, we need to add the force gathered by the lever arm 
	*/

	float elasticity_average = (corpse_a->get_elasticity() + corpse_b->get_elasticity()) * 0.5f;
	sf::Vector2f normal = ftn::Normalize(force);

	/* Test if the collision is asymetric (Fixed/Not Fixed) / or if the two corpses are Fixed */
	if (corpse_a->get_fixed() || corpse_b->get_fixed()) { 

		if (!corpse_a->get_fixed()) { /* A not fixed | B is fixed */

			//float reflexion_angle = ftn::Angle(corpse_a->get_last_pos(), corpse_a->get_pos(), corpse_a->get_pos() + normal);
			float speed_a = corpse_a->get_speed();
			corpse_a->Move(force); /* prevent overlap but alter the speed */
			
			//ftn::Rotate(normal, -reflexion_angle);
			//corpse_a->Move(normal);

		} else if (!corpse_b->get_fixed()) { /* A is fixed | B not fixed */

			//float reflexion_angle = ftn::Angle(corpse_b->get_last_pos(), corpse_b->get_pos(), corpse_b->get_pos() + normal);
			//float speed_b = corpse_b->get_speed();
			corpse_b->Move(-force); /* prevent overlap but alter the speed */

			//ftn::Rotate(normal, -reflexion_angle);
			//corpse_b->Move(-normal);
			
		}

	} else {
		/* Acceleration : F = ma */
		/*
		float mass_a = corpse_a->get_mass();
		float mass_b = corpse_b->get_mass();
		float mass_sum = mass_a + mass_b;

		float speed_a = corpse_a->get_speed();
		float speed_b = corpse_b->get_speed();
		
		sf::Vector2f velocity_a = corpse_a->get_velocity();
		sf::Vector2f velocity_b = corpse_b->get_velocity();
		*/
		corpse_a->Move(force*0.5f); /* prevent overlap but alter the speed */
		corpse_b->Move(-force*0.5f); /* prevent overlap but alter the speed */
		
		/* Test using the old distance (we changed it to avoid overlapping) */
		/*
		sf::Vector2f tangent = sf::Vector2f(-normal.x, normal.y);
		
		float tangent_a = velocity_a.x * tangent.x + velocity_a.y * tangent.y;
		float tangent_b = velocity_b.x * tangent.x + velocity_b.y * tangent.y;

		float normal_a = velocity_a.x * normal.x + velocity_a.y * normal.y;
		float normal_b = velocity_b.x * normal.x + velocity_b.y * normal.y;

		float result_a = (normal_a * (mass_a - mass_b) + 2.0f * mass_b * normal_b)/mass_sum;
		float result_b = (normal_b * (mass_b - mass_a) + 2.0f * mass_a * normal_a)/mass_sum;

		corpse_a->impulse_vector(tangent * tangent_a + normal * result_a);
		corpse_b->impulse_vector(tangent * tangent_b + normal * result_b);
		*/

		/*
		float mass_a = corpse_a->get_mass();
		float mass_b = corpse_b->get_mass();
		float mass_sum = mass_a + mass_b;

		float speed_a = corpse_a->get_speed();
		float speed_b = corpse_b->get_speed();

		float result_a = (speed_a * (mass_a - mass_b) + 2 * mass_b * speed_b)/mass_sum;
		float result_b = (speed_b * (mass_b - mass_a) + 2 * mass_a * speed_a)/mass_sum;

		sf::Vector2f normal_a = normal;
		sf::Vector2f normal_b = normal;

		float reflexion_angle_a = ftn::Angle(corpse_a->get_last_pos(), corpse_a->get_pos(), corpse_a->get_pos() + normal);
		float reflexion_angle_b = ftn::Angle(corpse_b->get_last_pos(), corpse_b->get_pos(), corpse_b->get_pos() + normal);
		
		ftn::Rotate(normal_a, -reflexion_angle_a);
		ftn::Rotate(normal_b, -reflexion_angle_b);

		corpse_a->impulse_vector(normal * result_a * elasticity_average);
		corpse_b->impulse_vector(-normal * result_b * elasticity_average);
		*/
		//sf::Vector2f normal = ftn::Normalize(vect_force);
		
		/* 
								-(1+e) * (v2-v1) * normal
		jr = ________________________________________________________________

			 m1 + m2 + (I1(r1 x normal) x r1 + I2(r2 x normal) x r2) * normal
		*/
		/*
		sf::Vector2f relative_velocity = corpse_b->get_velocity() - corpse_a->get_velocity();
		float vector_normal = ftn::Dot(relative_velocity, normal);
		if (vector_normal > 0) { return; }

		float e = 0.1; //temp
		float normal_mass_a = 1/corpse_a->get_mass();
		float normal_mass_b = 1/corpse_b->get_mass();
		float mass_sum = corpse_a->get_mass() + corpse_b->get_mass();
		float impulse_magnitude = (-(1+normal_elasticity)*vector_normal)/(normal_mass_a+normal_mass_b);
		sf::Vector2f impulse = impulse_magnitude*normal;
		
		corpse_a->Move(force * (corpse_a->get_mass()/mass_sum));
		corpse_b->Move(-force * (corpse_b->get_mass()/mass_sum));
		*/

		/* Rotation : τ = Iα */
		// Mass response is not evenly distributed among the corpses
		
	}
}

sf::Color Corpse::get_color() const { return this->color; }
void Corpse::set_color(sf::Color color) { this->color = color; }

sf::Vector2f Corpse::get_pos() const { return this->current_pos; }
float Corpse::get_pos_x() const { return this->current_pos.x; }
float Corpse::get_pos_y() const { return this->current_pos.y; }

void Corpse::set_pos(sf::Vector2f pos) { this->current_pos = pos; }
void Corpse::set_pos_x(float pos_x) { this->current_pos.x = pos_x; }
void Corpse::set_pos_y(float pos_y) { this->current_pos.y = pos_y; }

sf::Vector2f Corpse::get_last_pos() const { return this->last_pos; }
float Corpse::get_last_pos_x() const { return this->last_pos.x; }
float Corpse::get_last_pos_y() const { return this->last_pos.y; }

void Corpse::set_last_pos(sf::Vector2f pos) { this->last_pos = pos; }
void Corpse::set_last_pos_x(float pos_x) { this->last_pos.x = pos_x; }
void Corpse::set_last_pos_y(float pos_y) { this->last_pos.y = pos_y; }

void Corpse::impulse_vector(sf::Vector2f vector) {
	this->set_last_pos(this->get_pos());
	this->Move(vector);
}

void Corpse::transpose_pos(sf::Vector2f pos) {
	this->set_pos(this->get_pos() + pos);
	this->set_last_pos(this->get_last_pos() + pos);
}

sf::Vector2f Corpse::get_velocity() const { return this->get_pos() - this->get_last_pos(); }
float Corpse::get_speed() const { return ftn::Length(this->get_velocity()); }

float Corpse::get_rotation() const { return this->current_rotation; }
void Corpse::set_rotation(float current_rotation){ this->current_rotation = current_rotation; }

float Corpse::get_last_rotation() const { return this->last_rotation; }
void Corpse::set_last_rotation(float last_rotation) { this->last_rotation = last_rotation; }

float Corpse::get_motor_rotation() const { return this->motor_rotation; }
void Corpse::set_motor_rotation(float motor_rotation) { this->motor_rotation = motor_rotation; }

float Corpse::get_rotation_speed() const { return this->get_rotation() - this->get_last_rotation(); }

void Corpse::set_elasticity(float elasticity) { this->elasticity = std::max(elasticity, 0.0f); }
float Corpse::get_elasticity() const { return this->elasticity; }
float Corpse::get_mass() const { return this->mass; }

bool Corpse::Equals(const Corpse* other) { 	
	if (this->get_id() != other->get_id()) { return false; }
	if (this->get_class() != other->get_class()) { return false; }
	if (this->get_fixed() != other->get_fixed()) { return false; }
	if (this->get_etherial() != other->get_etherial()) { return false; }
	return true;
}

inline bool Corpse::operator==(const Corpse* other) {
	return this->Equals(other);
}

}