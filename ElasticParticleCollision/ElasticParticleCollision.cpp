#include <raylib.h>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <string>
#include <iostream>



struct Particle {
    float x, y;
    float speedX, speedY;
    float radius = 5;

    Color color = PURPLE;

    void Draw() {
        DrawCircle(x, y, radius, color);			//centers a ball to the middle by dividing its screen dimensions by half
    }
};

bool CheckCollision(Particle* p1, Particle* p2) {
    float dx = p2->x - p1->x;
    float dy = p2->y - p1->y;
    float distanceSquared = dx * dx + dy * dy;
    return distanceSquared <= (p1->radius + p2->radius) * (p1->radius + p2->radius);
}

void HandleCollision(Particle* p1, Particle* p2) {
    float dx = p2->x - p1->x;
    float dy = p2->y - p1->y;
    float distance = sqrt(dx * dx + dy * dy);
    float nx = dx / distance;
    float ny = dy / distance;
    float tx = -ny;
    float ty = nx;
    float dvx = p2->speedX - p1->speedX;
    float dvy = p2->speedY - p1->speedY;
    float dpNormal = dvx * nx + dvy * ny;
    float imp = (2.0f * dpNormal) / (1 + 1);
    p1->speedX += imp * nx;
    p1->speedY += imp * ny;
    p2->speedY -= imp * ny;
    p2->speedX -= imp * nx;
    float overlap = (p1->radius + p2->radius) - distance;
    p1->x -= overlap * nx * 0.5f;
    p1->y -= overlap * ny * 0.5f;
    p2->x += overlap * nx * 0.5f;
    p2->y += overlap * ny * 0.5f;
    p1->color = RED;
    p2->color = BLUE;
}

void AddNewParticle(std::vector<Particle*>& particles) {
    if (IsKeyPressed(KEY_SPACE)) {
        Particle* newParticle = new Particle();
        newParticle->x = GetScreenWidth() / 2.0f;
        newParticle->y = GetScreenHeight() / 2.0f;
        newParticle->speedX = 300;
        newParticle->speedY = 300;
        particles.push_back(newParticle);
    }
}

void CenterAllParticles(std::vector<Particle*>& particles) {
    if (IsKeyDown(KEY_E)) {
        float centerX = (GetScreenWidth() / 5 + GetScreenWidth()) / 2.0f;
        float centerY = GetScreenHeight() / 2.0f;
        for (int i = 0; i < particles.size(); i++) {
            float dx = centerX - particles[i]->x;
            float dy = centerY - particles[i]->y;
            float length = sqrt(dx * dx + dy * dy);
            if (length != 0) {
                dx /= length;
                dy /= length;
            }
            particles[i]->x += dx * 0.3f;
            particles[i]->y += dy * 0.3f;
        }
    }
}

void ParticleCollision(std::vector<Particle*>& particles) {
    for (int i = 0; i < particles.size(); i++) {
        particles[i]->x += particles[i]->speedX * GetFrameTime();
        particles[i]->y += particles[i]->speedY * GetFrameTime();
        if (particles[i]->y < 0) {
            particles[i]->y = 0;
            particles[i]->speedY *= -1;
        }
        if (particles[i]->y > GetScreenHeight()) {
            particles[i]->y = GetScreenHeight();
            particles[i]->speedY *= -1;
        }
        if (particles[i]->x < GetScreenWidth() / 5) {
            particles[i]->x = GetScreenWidth() / 5;
            particles[i]->speedX *= -1;
        }
        if (particles[i]->x > GetScreenWidth()) {
            particles[i]->x = GetScreenWidth();
            particles[i]->speedX *= -1;
        }
    }
    for (int i = 0; i < particles.size(); i++) {
        for (int j = i + 1; j < particles.size(); j++) {
            if (CheckCollision(particles[i], particles[j])) {
                HandleCollision(particles[i], particles[j]);
            }
        }
    }
}

void Spawn50Particles(std::vector<Particle*>& particles) {
    if (IsKeyPressed(KEY_R)) {
        for (int i = 0; i < 50; i++) {
            int RandomXPos = GetRandomValue(GetScreenWidth() / 2, GetScreenWidth());
            int RandomYPos = GetRandomValue(0, GetScreenHeight());
            Particle* newParticle = new Particle();
            newParticle->x = RandomXPos;
            newParticle->y = RandomYPos;
            newParticle->speedX = 300;
            newParticle->speedY = 300;
            particles.push_back(newParticle);
        }
    }
}

int main()
{
    InitWindow(800, 600, "Particle Collision");		//intialize window

    std::string add = "Add Particles \n\nPress: Space";
    std::string center = "\n\nCenter Particles \n\nPress: E";
    std::string spawn = "\nSpawn 50 Particles \n\nPress: R";
    std::string pDelete = "Delete Particles\n\nPress: D";

    std::vector<Particle*> particles;
    Particle* particle = new Particle();
    particle->x = GetScreenWidth() / 2.0f;
    particle->y = GetScreenHeight() / 2.0f;
    particle->speedX = 300;
    particle->speedY = 300;
    particles.push_back(particle);




    while (!WindowShouldClose()) {    //run window till closed basic screen loop

        AddNewParticle(particles);
        CenterAllParticles(particles);
        ParticleCollision(particles);
        Spawn50Particles(particles);

        if (IsKeyPressed(KEY_D)) {
            for (Particle* p : particles) {
                delete p; // Freeing dynamically allocated memory
            }
            particles.clear();
        }

        std::string counter = "Particles: " + std::to_string(particles.size());

        BeginDrawing();
        ClearBackground(WHITE);			//sets background color
        for (int i = 0; i < particles.size(); i++) {
            particles[i]->Draw();
        }
        DrawRectangle(0, 0, GetScreenWidth() / 5, GetScreenHeight(), BLACK); //Menu Background
        DrawText(add.c_str(), GetScreenWidth() / 45, GetScreenHeight() / 15, GetScreenHeight() / 40, PURPLE);   //add
        DrawText(center.c_str(), GetScreenWidth() / 45, GetScreenHeight() / 9, GetScreenHeight() / 40, LIME);   //center
        DrawText(spawn.c_str(), GetScreenWidth() / 45, GetScreenHeight() / 4, GetScreenHeight() / 40, ORANGE);	//Spawn
        DrawText(pDelete.c_str(), GetScreenWidth() / 45, GetScreenHeight() / 2.5, GetScreenHeight() / 40, SKYBLUE);	//DELETE PARTICLES
        DrawText(counter.c_str(), GetScreenWidth() / 45, GetScreenHeight() / 2, GetScreenHeight() / 40, GOLD);	//Counter

        DrawFPS(10, 10);				//fps count
        EndDrawing();
    }

    CloseWindow();


}