#include "Databases/Database.hpp"

Database::Database() {

}

Database::~Database() {

}

int Database::getEpochSize() {
    return epochSize;
}

int Database::getTestSize() {
    return testSize;
}