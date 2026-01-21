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

    const std::string& getName() const { return name; }
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

    bool isValidWeaponIndex(int index) const {
        return index >= 0 && index < static_cast<int>(weapons.size());
    }

    void equipWeaponByIndex(Character& character, int index) {
        if (!isValidWeaponIndex(index)) return;
        character.equipWeapon(&weapons[index]);
    }

public:
    GameManager(const Player& p, const Enemy& e)
        : player(p), enemy(e) {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
    }

    void addWeapon(const Weapon& weapon) {
        weapons.push_back(weapon);
    }

    void equipPlayerWeapon(int index) {
        equipWeaponByIndex(player, index);
    }

    void equipEnemyWeapon(int index) {
        equipWeaponByIndex(enemy, index);
    }

    Weapon* equipRandomWeapon(Character& character) {
        if (weapons.empty()) return nullptr;

        int randomIndex = std::rand() % weapons.size();
        Weapon* selectedWeapon = &weapons[randomIndex];
        character.equipWeapon(selectedWeapon);
        return selectedWeapon;
    }

    void randomlyHealPlayer() {
        int healAmount = std::rand() % 50 + 1;
        player.heal(healAmount);
    }

    int startGame() {
        std::cout << "Game started: "
            << player.getName() << " vs "
            << enemy.getName() << "\n";

        if (!player.hasWeapon()) equipRandomWeapon(player);
        if (!enemy.hasWeapon()) equipRandomWeapon(enemy);

        while (player.isAlive() && enemy.isAlive()) {
            if (player.hasWeapon() && enemy.hasWeapon()) {
                player.attack(enemy);
                if (!enemy.isAlive()) break;

                enemy.attack(player);
                if (!player.isAlive()) break;
            }
            else {
                std::cout << "Weapon not equipped. Cannot fight.\n";
                break;
            }

            randomlyHealPlayer();
        }

        if (player.getHealth() <= 0) {
            std::cout << player.getName() << " has been defeated.\n";
            return 1;
        }
        else {
            std::cout << enemy.getName() << " has been defeated.\n";
            return 0;
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
