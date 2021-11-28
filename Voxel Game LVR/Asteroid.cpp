#include "Asteroid.h"
#include "Prefabs.h"

Asteroid::Asteroid(glm::vec3 location, glm::vec3 vectr, int size)
	: position(location)
	, orientation(glm::vec3(NULL))
	, aligner(new Entity(NULL, NULL, location))
	, offsets(NULL)
	, selfDirection(NULL)
	, selfSpeed(vectr.length())
	, stage(size)
{
	std::vector<Texture*> textures;
	textures.push_back(new Texture("textures/rocky.tga", GL_REPEAT, GL_LINEAR));
	textures.push_back(new Texture("textures/debug2.tga", GL_CLAMP_TO_EDGE, GL_LINEAR));

	std::vector<Material*> materials;
	materials.push_back(new Material(textures[0]));
	materials.push_back(new Material(textures[1]));

	std::vector<Mesh*> meshes;
	// Model
	meshes.push_back(CreateTexturedCube(4));
	meshes.push_back(CreateTexturedCube(2));

	// Hitbox
	meshes.push_back(CreateWireframeBox(4.1, 4.1, 5.2));
	meshes.push_back(CreateWireframeBox(5.2, 4.1, 4.1));
	meshes.push_back(CreateWireframeBox(4.1, 5.2, 4.1));

	// Model
	entities.push_back(new Entity(meshes[0], materials[0], Transform(position.x, position.y, position.z))); // Base
	entities.push_back(new Entity(meshes[1], materials[0], Transform(position.x + 1.5, position.y, position.z)));
	entities.push_back(new Entity(meshes[1], materials[0], Transform(position.x - 1.5, position.y, position.z)));
	entities.push_back(new Entity(meshes[1], materials[0], Transform(position.x, position.y + 1.5, position.z)));
	entities.push_back(new Entity(meshes[1], materials[0], Transform(position.x, position.y - 1.5, position.z)));
	entities.push_back(new Entity(meshes[1], materials[0], Transform(position.x, position.y, position.z + 1.5)));
	entities.push_back(new Entity(meshes[1], materials[0], Transform(position.x, position.y, position.z - 1.5)));
	// Hitbox
	hitboxes.push_back(new Entity(meshes[2], materials[1], Transform(position.x, position.y, position.z)));
	hitboxes.push_back(new Entity(meshes[3], materials[1], Transform(position.x, position.y, position.z)));
	hitboxes.push_back(new Entity(meshes[4], materials[1], Transform(position.x, position.y, position.z)));

	glm::vec3 relatedLoc(0, 0, 0);

	for (int i = 0; i < entities.size(); i++) {
		relatedLoc.x = entities[i]->getPosition().x - position.x;
		relatedLoc.y = entities[i]->getPosition().y - position.y;
		relatedLoc.z = entities[i]->getPosition().z - position.z;

		offsets.push_back(relatedLoc);
	}

	for (int i = 0; i < hitboxes.size(); i++) {
		relatedLoc.x = hitboxes[i]->getPosition().x - position.x;
		relatedLoc.y = hitboxes[i]->getPosition().y - position.y;
		relatedLoc.z = hitboxes[i]->getPosition().z - position.z;

		hbOffsets.push_back(relatedLoc);
	}

	// Set direction to unit vector
	selfDirection = glm::vec3(vectr.x / vectr.length(), vectr.y / vectr.length(), vectr.z / vectr.length());
}

void Asteroid::adjustOrientation(glm::quat orientation) {

	aligner->setOrientation(orientation);
	for (int i = 0; i < entities.size(); i++) {
		aligner->setPosition(position);
		aligner->translateLocal(offsets[i]);
		entities[i]->setPosition(aligner->getPosition());
		entities[i]->setOrientation(orientation);
	}

	// rotating the hitboxes
	for (int i = 0; i < hitboxes.size(); i++) {
		aligner->setPosition(position);
		aligner->translateLocal(hbOffsets[i]);
		hitboxes[i]->setPosition(aligner->getPosition());
		hitboxes[i]->setOrientation(orientation);
	}

}

void Asteroid::spin() {
	aligner->setOrientation(entities[0]->getOrientation());
	aligner->rotate(2 * selfSpeed, glm::vec3(0,0,1));
	adjustOrientation(aligner->getOrientation());
}

void Asteroid::update() {
	spin();
}
