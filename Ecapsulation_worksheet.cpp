#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

class Weapon {
private:
    std::string name;
    int damage;

public:
    Weapon(const std::string& weaponName, int weaponDamage)
        : name(weaponName), damage(weaponDamage) {}

    std::string getName() const { return name; }
    int getDamage() const { return damage; }

    void setDamage(int newDamage) { damage = newDamage; }
};


class Character {
private:
    std::string name;
    int health;
    int strength;
    Weapon* currentWeapon;

public:
    Character(const std::string& characterName, int characterHealth, int characterStrength)
        : name(characterName),
        health(characterHealth),
        strength(characterStrength),
        currentWeapon(nullptr) {
    }

    const std::string& getName() const { return name; }
    int getHealth() const { return health; }

    bool isAlive() const { return health > 0; }
    bool hasWeapon() const { return currentWeapon != nullptr; }

    void equipWeapon(Weapon* weapon) {
        currentWeapon = weapon;
    }

    void heal(int amount) {
        if (health > 0) {
            health += amount;
            std::cout << "Player healed by " << amount << " points.\n";
        }
    }

    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) health = 0;

        std::cout << name << "take damage " << damage << "\n";
    }

    void attack(Character& target) {
        if (!currentWeapon) return;

        std::cout << name << " attacks " << target.getName()
            << " with " << currentWeapon->getName() << "\n";

        target.takeDamage(currentWeapon->getDamage() * strength);
        std::cout << target.getName() << " health: "
            << target.getHealth() << "\n";
    }
};

class Player : public Character {
public:
    Player(const std::string& playerName, int playerHealth, int characterStrength)
        :Character(playerName, playerHealth, characterStrength) {}
};

class Enemy : public Character {
public:
    Enemy(const std::string& EnemyName, int EnemyHealth, int characterStrength)
        :Character(EnemyName, EnemyHealth, characterStrength) {}

};

class GameManager {
private:
    Player player;
    Enemy enemy;
    std::vector<Weapon> weapons;

public:
    GameManager(const Player& p, const Enemy& e)
        : player(p), enemy(e) {
        std::srand(std::time(0)); // Seed for random number generation
    }

    void addWeapon(const Weapon& weapon) {
        weapons.push_back(weapon);
    }

    int startGame() {
        std::cout << "Game started: " << player.getName() << " vs " << enemy.getName() << "\n";

        // Player and enemy health checks
        while (player.getHealth() > 0 && enemy.getHealth() > 0) {
            Weapon* playerWeapon = player.getWeapon();
            Weapon * enemyWeapon = enemy.getWeapon();

            if (playerWeapon != nullptr && enemyWeapon != nullptr) {
                std::cout << player.getName() << " attacks " << enemy.getName() << " with " << playerWeapon->getName() << "\n";
				enemy.takeDamage(playerWeapon->getDamage() * player.strength);
                std::cout << enemy.getName() << " health: " << enemy.getHealth() << "\n";

                std::cout << enemy.getName() << " attacks " << player.getName() << " with " << enemyWeapon->getName() << "\n";
                player.takeDamage(enemyWeapon->getDamage() * enemy.strength);
                std::cout << player.getName() << " health: " << player.getHealth() << "\n";
            }
            else {
                std::cout << "Weapon not equipped. Cannot fight.\n";
                break;
            }
            randomlyHealPlayer();
        }
        equipRandomWeapon(player);
        equipRandomWeapon(enemy);


        
        if (player.getHealth() <= 0) {
            std::cout << player.getName() << " has been defeated.\n";
            return 1;
        }
        else if (enemy.getHealth() <= 0) {
            std::cout << enemy.getName() << " has been defeated.\n";
            return 0;
        }
    }

    void equipPlayerWeapon(int weaponIndex) {
        if (weaponIndex >= 0 && weaponIndex < weapons.size()) {
            player.currentWeapon=&weapons[weaponIndex];
        }
    }

    void equipEnemyWeapon(int weaponIndex) {
        if (weaponIndex >= 0 && weaponIndex < weapons.size()) {
            enemy.currentWeapon = &weapons[weaponIndex];
        }
    }


    Weapon* equipRandomWeapon(Character& character) {

        if (weapons.empty()) {
            return nullptr;
        }
        int randomIndex = std::rand() % weapons.size();
        Weapon* selectedWeapon = &weapons[randomIndex];
        character.currentWeapon = selectedWeapon;
        return selectedWeapon;
    }

    void randomlyHealPlayer() {
        int healAmount = std::rand() % 50 + 1; // heal between 1 and 50 point
        healPlayer(healAmount);
    }

    void healPlayer(int amount) {
        if (player.getHealth() > 0) {
            player.setHealth(player.getHealth() + amount);
            std::cout << "Player healed by " << amount << " points.\n";
        }
    }
};

// Main Function
int main() {

    Player player("Hero", 300, 2);
    Enemy enemy("Goblin", 150, 4);

    Weapon sword("Sword", 15);
    Weapon axe("Axe", 20);
    Weapon dagger("Dagger", 10);
    Weapon bow("Bow", 25);

    GameManager game(player, enemy);

    game.addWeapon(sword);
    game.addWeapon(axe);
    game.addWeapon(dagger);
    game.addWeapon(bow);

    // Equip weapons
    game.equipPlayerWeapon(0); // Equip sword to player
    game.equipEnemyWeapon(1);  // Equip axe to enemy


    game.startGame();

    return 0;
}
