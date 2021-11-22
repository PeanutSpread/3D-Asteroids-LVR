#include "Projectile.h"

Projectile::Projectile(glm::vec3 location, glm::quat orientation)
	: position(location)
	, orientation(orientation)
	, aligner(new Entity(NULL, NULL, location))
	, offsets(NULL)
{
	std::vector<Texture*> textures;
	textures.push_back(new Texture("textures/debug.tga", GL_REPEAT, GL_LINEAR));

	std::vector<Material*> materials;
	materials.push_back(new Material(textures[0]));

	std::vector<Mesh*> meshes;
	meshes.push_back(CreateChunkyCone(0.5, 0.75, 8));
	meshes.push_back(CreateChunkyTexturedCylinder(0.5, 1.5, 8));
	meshes.push_back(CreateChunkyTexturedCylinder(0.4, 0.5, 8));

	entities.push_back(new Entity(meshes[0], materials[0], Transform(position.x, position.y, position.z))); // Front Cone
	entities.push_back(new Entity(meshes[1], materials[0], Transform(position.x, position.y - 1.1, position.z)));
	entities.push_back(new Entity(meshes[2], materials[0], Transform(position.x, position.y - 2.0, position.z)));

	glm::vec3 relatedLoc(0, 0, 0);

	for (int i = 0; i < entities.size(); i++) {
		relatedLoc.x = entities[i]->getPosition().x - position.x;
		relatedLoc.y = entities[i]->getPosition().y - position.y;
		relatedLoc.z = entities[i]->getPosition().z - position.z;

		offsets.push_back(relatedLoc);
	}

	adjustOrientation();
}

void Projectile::adjustOrientation() {

	aligner->setOrientation(orientation);
	for (int i = 0; i < entities.size(); i++) {
		aligner->setPosition(position);
		aligner->translateLocal(offsets[i]);
		entities[i]->setPosition(aligner->getPosition());
		entities[i]->setOrientation(orientation);
	}

}

void Projectile::update(float dt) {
	float speed = 75;
	float disp = speed * dt;

	glm::vec3 displacement(0, 0, 0);

	displacement.y = disp;

	for (int i = 0; i < entities.size(); i++) {
		entities[i]->translateLocal(displacement);
	}
}
