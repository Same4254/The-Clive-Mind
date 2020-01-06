class NetworkInformation {
    private:
        double learningRate, velocityCoefficient;
        int batchSize, batchIndex;
        int amountOfLayers;

    public:
        NetworkInformation(int amountOfLayers) {
            this->amountOfLayers = amountOfLayers;

            learningRate = 0.0;
            velocityCoefficient = 0.0;
            batchSize = 1;
            batchIndex = 0;
        }

        ~NetworkInformation() {
            
        }

        int getAmountOfLayers() { return amountOfLayers; }

        double getLearningRate() { return learningRate; }
        void setLearningRate(double learningRate) { this->learningRate = learningRate; }

        double getVelocityCoefficient() { return velocityCoefficient; }
        void setVelocityCoefficient(double velocityCoefficient) { this->velocityCoefficient = velocityCoefficient; }

        int getBatchSize() { return batchSize; }
        void setBatchSize(int batchSize) { this->batchSize = batchSize; }

        bool incrementBatchIndex() {
            batchIndex++;

            if(batchIndex == batchSize) {
                batchIndex = 0;
                return true;
            }

            return false;
        }

        int getBatchIndex() { return batchIndex; }
};