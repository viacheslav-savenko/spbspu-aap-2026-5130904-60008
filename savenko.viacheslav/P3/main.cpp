#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

namespace savenko {

void readMatrixFromFile(const std::string &input_filename, int *matrix, int max_capacity, int &rows, int &cols);
void applyLayerIncrement(int *matrix, int rows, int cols);
void writeMatrixToFile(const std::string &output_filename, const int *matrix, int rows, int cols);

void processFileWithStaticArray(const std::string &input_filename, const std::string &output_filename);
void processFileWithDynamicArray(const std::string &input_filename, const std::string &output_filename);

}

int main(int argc, char *argv[]) 
{
  if (argc != 4) {
    std::cerr << "Error: Invalid number of arguments. Expected: ./lab num input output\n";
    return 1;
  }

  try {
    std::string parameter_string = argv[1];
    std::string input_filename = argv[2];
    std::string output_filename = argv[3];

    if (parameter_string != "1" && parameter_string != "2") {
      std::cerr << "Error: First parameter must be 1 or 2.\n";
      return 1;
    }
    
    int parameter = std::stoi(parameter_string);

    if (parameter == 1) {
      savenko::processFileWithStaticArray(input_filename, output_filename);
    } 
    else if (parameter == 2) {
      savenko::processFileWithDynamicArray(input_filename, output_filename);
    }
  } 
  catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  } 
  catch (int error_code) {
    return error_code;
  }

  return 0;
}

void savenko::readMatrixFromFile(const std::string &input_filename, int *matrix, int max_capacity, int &rows, int &cols) 
{
  std::ifstream inFile(input_filename);
  if (!inFile.is_open()) {
    std::cerr << "Error: Failed to open the file for reading!\n";
    throw 2; 
  }

  if (!(inFile >> rows >> cols) || rows <= 0 || cols <= 0) {
    std::cerr << "Error: Invalid matrix dimensions.\n";
    throw 2;
  }

  int total_elements = rows * cols;
  if (total_elements > max_capacity) {
    std::cerr << "Error: Matrix exceeds allocated capacity.\n";
    throw 2;
  }

  int element_count = 0;
  int current_number = 0;

  while (inFile >> current_number) {
    if (element_count >= total_elements) {
      std::cerr << "Error: Extraneous elements found.\n";
      throw 2;
    }

    matrix[element_count] = current_number;
    element_count = element_count + 1;
  }

  if (!inFile.eof() && inFile.fail()) {
    std::cerr << "Error: Invalid character or overflow.\n";
    throw 2;
  }

  if (element_count < total_elements) {
    std::cerr << "Error: Not enough elements.\n";
    throw 2;
  }

  inFile.close();
}

void savenko::applyLayerIncrement(int *matrix, int rows, int cols) 
{
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      int dist_top = i;
      int dist_bottom = rows - 1 - i;
      int dist_left = j;
      int dist_right = cols - 1 - j;

      int layer = std::min({dist_top, dist_bottom, dist_left, dist_right});
      int index_1d = i * cols + j;

      matrix[index_1d] = matrix[index_1d] + (layer + 1); // Работаем с matrix
    }
  }
}

void savenko::writeMatrixToFile(const std::string &output_filename, const int *matrix, int rows, int cols) 
{
  std::ofstream outFile(output_filename);
  if (!outFile.is_open()) {
    std::cerr << "Error: Failed to open file for writing!\n";
    throw 2;
  }

  outFile << rows << " " << cols;

  int total_elements = rows * cols;
  for (int i = 0; i < total_elements; ++i) {
    outFile << " " << matrix[i];
  }
  
  outFile << "\n";
  outFile.close();
}

void savenko::processFileWithStaticArray(const std::string &input_filename, const std::string &output_filename) 
{
  const int max_elements = 10000;
  int matrix[max_elements] = {};
  int rows = 0;
  int cols = 0;

  readMatrixFromFile(input_filename, matrix, max_elements, rows, cols);
  applyLayerIncrement(matrix, rows, cols);
  writeMatrixToFile(output_filename, matrix, rows, cols);
}

void savenko::processFileWithDynamicArray(const std::string &input_filename, const std::string &output_filename) 
{
  int rows = 0;
  int cols = 0;

  {
    std::ifstream inFile(input_filename);
    if (!inFile.is_open() || !(inFile >> rows >> cols) || rows <= 0 || cols <= 0) {
      std::cerr << "Error: Invalid matrix or dimensions.\n";
      throw 2;
    }
  }

  int total_elements = rows * cols;
  int *matrix = new int[total_elements]();

  try {
    int rows_check = 0;
    int cols_check = 0;

    readMatrixFromFile(input_filename, matrix, total_elements, rows_check, cols_check);
    applyLayerIncrement(matrix, rows, cols);
    writeMatrixToFile(output_filename, matrix, rows, cols);
  } 
  catch (int error_code) {
    delete[] matrix;
    throw error_code;
  }

  delete[] matrix;
}
