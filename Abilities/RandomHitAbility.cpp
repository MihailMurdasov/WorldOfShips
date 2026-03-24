#include "RandomHitAbility.hpp"

RandomHit::RandomHit(GameField& field) : field(field) {}

void RandomHit::apply(ResultOfUsingAbilities& results) {
    int shipIndices = field.getManager().getShips().size();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> randomShip(0,shipIndices-1);
    int randomShipIndex = randomShip(gen);
    std::uniform_int_distribution<> randomSegment(0,static_cast<int>(field.getManager().getShips()[randomShipIndex].getLength()) - 1);
    int randomSegmentIndex = randomSegment(gen);
    while (field.getManager().getShips()[randomShipIndex].getSegmentState(randomSegmentIndex) != SegmentState::Intact &&
           field.getManager().getShips()[randomShipIndex].getSegmentState(randomSegmentIndex) != SegmentState::Damaged) {
        randomShipIndex = randomShip(gen);
        randomSegmentIndex = randomSegment(gen);
    }

    field.getManager().getShips()[randomShipIndex].hitSegment(randomSegmentIndex, 1);

    if (field.getManager().getShips()[randomShipIndex].isDestroyed()) {
        results.setRandomHitResult(RandomHitResult::KILL);
    } else {
        results.setRandomHitResult(RandomHitResult::HIT);
    }

    results.setLastAbilityUsed(LastAbilityUsed::RandomHit);
}