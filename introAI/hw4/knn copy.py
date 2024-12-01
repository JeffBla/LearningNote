import csv
import math
import random

random.seed(0)


class DistanceMetric:

    @classmethod
    def distance(cls, row1, row2, root):
        return DistanceMetric._euclidean_distance(row1, row2, root)

    @classmethod
    def _euclidean_distance(cls, row1, row2, root):
        distance = 0.0
        for i in range(len(row1) - 1):
            distance += (row1[i] - row2[i])**root
        return DistanceMetric._root(distance, root)

    @classmethod
    def _root(cls, value, n):
        if value < 0:
            return -1 * (abs(value)**(1 / n))
        return value**(1 / n)


class LabelConverter:

    @classmethod
    def convert_binary(cls, value):
        return 1 if value == 'Yes' else 0

    @classmethod
    def convert_numeric(cls, value):
        try:
            return float(value)
        except ValueError:
            return None

    @classmethod
    def convert_multi_value(cls, value):
        if value == 'No phone service' or value == 'No internet service':
            return 0
        elif value == 'Yes':
            return 1
        else:
            return 2


class DataPath:

    def __init__(self, train_path, train_gt_path, val_path, val_gt_path,
                 test_path):
        self.train_path = train_path
        self.train_gt_path = train_gt_path
        self.val_path = val_path
        self.val_gt_path = val_gt_path
        self.test_path = test_path

    def getPath(self):
        return self.train_path, self.train_gt_path, self.val_path, self.val_gt_path, self.test_path


class Dataset:

    def __init__(self, train_data, train_gt_data, val_data, val_gt_data,
                 test_data):
        self.train_data = train_data
        self.train_gt_data = train_gt_data
        self.val_data = val_data
        self.val_gt_data = val_gt_data
        self.test_data = test_data

    def getData(self):
        return self.train_data, self.train_gt_data, self.val_data, self.val_gt_data, self.test_data


class DataPreprocessor:

    def __init__(self, dataPath):
        self.dataset = self.dataImport(dataPath)

    def process(self):
        self.dataLabeling()
        self.dataStandardization()
        return self.dataset

    def dropRow(self, data, idx):
        return [row for i, row in enumerate(data) if i != idx]

    def dataImport(self, dataPath):
        with open(dataPath.train_path, 'r') as file:
            train_reader = csv.reader(file)
            train_data = [row for row in train_reader]
            train_data = self.dropRow(train_data, 0)
        with open(dataPath.train_gt_path, 'r') as file:
            train_gt_reader = csv.reader(file)
            train_gt_data = [row for row in train_gt_reader]
            train_gt_data = self.dropRow(train_gt_data, 0)

        with open(dataPath.val_path, 'r') as file:
            val_reader = csv.reader(file)
            val_data = [row for row in val_reader]
            val_data = self.dropRow(val_data, 0)
        with open(dataPath.val_gt_path, 'r') as file:
            val_gt_reader = csv.reader(file)
            val_gt_data = [row for row in val_gt_reader]
            val_gt_data = self.dropRow(val_gt_data, 0)

        with open(dataPath.test_path, 'r') as file:
            test_reader = csv.reader(file)
            test_data = [row for row in test_reader]
            test_data = self.dropRow(test_data, 0)

        self.dataset = Dataset(train_data, train_gt_data, val_data,
                               val_gt_data, test_data)
        return self.dataset

    def dataLabeling(self):
        for data in [
                self.dataset.train_data, self.dataset.val_data,
                self.dataset.test_data
        ]:
            for row in data:
                row[1] = LabelConverter.convert_binary(row[1])  # SeniorCitizen
                row[2] = LabelConverter.convert_binary(row[2])  # Partner
                row[3] = LabelConverter.convert_binary(row[3])  # Dependents
                row[5] = LabelConverter.convert_binary(row[5])  # PhoneService
                row[6] = LabelConverter.convert_multi_value(
                    row[6])  # MultipleLines
                row[15] = LabelConverter.convert_binary(
                    row[15])  # PaperlessBilling
                row[8] = LabelConverter.convert_multi_value(
                    row[8])  # OnlineSecurity
                row[9] = LabelConverter.convert_multi_value(
                    row[9])  # OnlineBackup
                row[10] = LabelConverter.convert_multi_value(
                    row[10])  # DeviceProtection
                row[11] = LabelConverter.convert_multi_value(
                    row[11])  # TechSupport
                row[12] = LabelConverter.convert_multi_value(
                    row[12])  # StreamingTV
                row[13] = LabelConverter.convert_multi_value(
                    row[13])  # StreamingMovies

                # Convert numeric features
                row[4] = LabelConverter.convert_numeric(row[4])  # tenure
                row[17] = LabelConverter.convert_numeric(
                    row[17])  # MonthlyCharges
                row[18] = LabelConverter.convert_numeric(
                    row[18])  # TotalCharges

                # Convert PaymentMethod
                if row[16] == 'Electronic check':
                    row[16] = 0
                elif row[16] == 'Bank transfer (automatic)':
                    row[16] = 1
                elif row[16] == 'Credit card (automatic)':
                    row[16] = 2
                elif row[16] == 'Mailed check':
                    row[16] = 3

                # Convert InternetService
                if row[7] == 'Fiber optic':
                    row[7] = 2
                elif row[7] == 'DSL':
                    row[7] = 1
                elif row[7] == 'No':
                    row[7] = 0

                # Convert Gender
                if row[0] == 'Male':
                    row[0] = 1
                elif row[0] == 'Female':
                    row[0] = 0

                # Convert Contract
                if row[14] == 'Month-to-month':
                    row[14] = 0
                elif row[14] == 'One year':
                    row[14] = 1
                elif row[14] == 'Two year':
                    row[14] = 2

        # Preprocess the ground truth data
        self.dataset.train_gt_data = [[
            LabelConverter.convert_binary(value) for value in row
        ] for row in self.dataset.train_gt_data]
        self.dataset.val_gt_data = [[
            LabelConverter.convert_binary(value) for value in row
        ] for row in self.dataset.val_gt_data]

        return self.dataset

    def dataStandardization(self):
        mean_std = self.dataset_mean_std(self.dataset.train_data)

        for data in [
                self.dataset.train_data, self.dataset.val_data,
                self.dataset.test_data
        ]:
            for row in data:
                for i in range(len(row)):
                    # Skip standardization if std is 0 to avoid division by zero
                    if mean_std[i][1] != 0:
                        row[i] = (row[i] - mean_std[i][0]) / mean_std[i][1]

    def dataset_mean_std(self, data):
        mean_std = []
        num_features = len(data[0])

        for i in range(num_features):
            col_values = [float(row[i])
                          for row in data]  # Ensure numeric values
            mean = sum(col_values) / len(col_values)

            # Calculate standard deviation with small epsilon to avoid division by zero
            squared_diff_sum = sum((x - mean)**2 for x in col_values)
            std = math.sqrt(squared_diff_sum / len(col_values)) + 1e-8

            mean_std.append([mean, std])

        return mean_std


class KNN:

    def __init__(self, k, root):
        self.root = root
        self.k = k

    def predict(self, train_data, train_gt_data, target_data):
        predictions = list()
        for target_row in target_data:
            neighbors = self.get_neighbors(train_data, target_row)
            prediction = self.get_response(neighbors, train_gt_data)
            predictions.append(prediction)
        return predictions

    def predict_val(self, dataset):
        return self.predict(dataset.train_data, dataset.train_gt_data,
                            dataset.val_data)

    def predict_test(self, dataset):
        return self.predict(dataset.train_data, dataset.train_gt_data,
                            dataset.test_data)

    def get_neighbors(self, train_data, test_row):
        distances = list()
        for idx, train_row in enumerate(train_data):
            dist = DistanceMetric.distance(test_row, train_row, self.root)
            distances.append((idx, dist))
        distances.sort(key=lambda tup: tup[1])
        neighbors = list()
        for i in range(self.k):
            neighbors.append(distances[i][0])
        return neighbors

    def get_response(self, neighbors, gt_data):
        class_values = [gt_data[idx][-1] for idx in neighbors]
        return max(set(class_values), key=class_values.count)


class EnsembleKNN:

    def __init__(self, n_models=5):
        self.models = []
        # Create diverse models with different k and root values
        for _ in range(n_models):
            k = random.randint(3, 15)  # Vary k from 3 to 7
            root = random.choice([1, 2, 3,
                                  4])  # Try different distance metrics
            self.models.append(KNN(k, root))

    def predict(self, train_data, train_gt_data, target_data):
        all_predictions = []

        # Get bootstrap samples
        for model in self.models:
            # Create bootstrap sample
            sample_indices = [
                random.randint(0,
                               len(train_data) - 1)
                for _ in range(len(train_data))
            ]
            bootstrap_train = [train_data[i] for i in sample_indices]
            bootstrap_gt = [train_gt_data[i] for i in sample_indices]

            # Get predictions from this model
            predictions = model.predict(bootstrap_train, bootstrap_gt,
                                        target_data)
            all_predictions.append(predictions)

        # Combine predictions by majority voting
        final_predictions = []
        for i in range(len(target_data)):
            votes = [pred[i] for pred in all_predictions
                     ]  # Get all predictions for this instance
            final_pred = max(set(votes), key=votes.count)  # Majority vote
            final_predictions.append([final_pred])

        return final_predictions

    def predict_val(self, dataset):
        return self.predict(dataset.train_data, dataset.train_gt_data,
                            dataset.val_data)

    def predict_test(self, dataset):
        return self.predict(dataset.train_data, dataset.train_gt_data,
                            dataset.test_data)


def csv_writer(file_path, data):
    # Convert numeric predictions to Yes/No strings
    formatted_data = [["Yes"] if pred == 1 else ["No"] for pred in data]

    # Write to CSV file
    with open(file_path, "w", newline='') as file:
        writer = csv.writer(file)
        writer.writerow(["Churn"])
        writer.writerows(formatted_data)


if __name__ == "__main__":
    # Hyperparameters
    k = 5
    root = 2

    test_path = "test.csv"
    train_path = "train.csv"
    val_path = "val.csv"

    train_gt_path = "train_gt.csv"
    val_gt_path = "val_gt.csv"

    dataPath = DataPath(train_path, train_gt_path, val_path, val_gt_path,
                        test_path)

    dataPreprocessor = DataPreprocessor(dataPath)
    dataset = dataPreprocessor.process()

    # Train the model
    # knn = KNN(k, root)
    # pred_val = knn.predict_val(dataset)
    # pred_test = knn.predict_test(dataset)

    # csv_writer("val_pred.csv", pred_val)

    # csv_writer("test_pred.csv", pred_val)

    # Train ensemble model instead of single KNN
    n_models = 100

    ensemble = EnsembleKNN(n_models)
    pred_val = ensemble.predict_val(dataset)

    csv_writer("val_pred_ensemble.csv", pred_val)
