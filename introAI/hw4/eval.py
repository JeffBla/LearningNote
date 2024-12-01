"""
use argparser, 
read two csv file: predicted and ground-truth, the first line is the field name, 
there will be only one field, compare them and calculate the accuracy and print the detail.
"""

import argparse
import csv


def calculate_accuracy(predicted_file, ground_truth_file, print_details=True):
    # Read the predicted values from the CSV file
    with open(predicted_file, 'r') as file:
        predicted_reader = csv.reader(file)
        predicted_values = [row[0] for row in predicted_reader]

    # Read the ground truth values from the CSV file
    with open(ground_truth_file, 'r') as file:
        ground_truth_reader = csv.reader(file)
        ground_truth_values = [row[0] for row in ground_truth_reader]

    # Ignore the first line of csv file
    predicted_values = predicted_values[1:]
    ground_truth_values = ground_truth_values[1:]

    # Calculate the accuracy
    total_samples = len(predicted_values)
    correct_predictions = sum(
        1 for pred, gt in zip(predicted_values, ground_truth_values)
        if pred == gt)
    accuracy = correct_predictions / total_samples
    if print_details:
        # Print the details
        print(f"Total samples: {total_samples}")
        print(f"Correct predictions: {correct_predictions}")
        print(f"Accuracy: {accuracy:.3f}")
    else:
        print(accuracy)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description=
        "Compare predicted and ground truth CSV files and calculate accuracy.")
    parser.add_argument("ground_truth_file",
                        help="Path to the ground truth CSV file")
    parser.add_argument("predicted_file",
                        help="Path to the predicted CSV file")
    parser.add_argument("--no-details",
                        dest="print_details",
                        action="store_false",
                        help="Do not print the details")
    args = parser.parse_args()

    calculate_accuracy(args.predicted_file, args.ground_truth_file,
                       args.print_details)
