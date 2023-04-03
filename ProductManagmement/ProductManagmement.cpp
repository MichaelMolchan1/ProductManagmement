/*
Michael Molchan
CIS 1202 101
April 3, 2023

A program that creates individual items and then enters them into a binary file. It also allows the user to modify them
*/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>

const int NAME_SIZE = 40;

// This is the structure used to store the data for each product
struct Product
{
    long number;
    char name[NAME_SIZE];
    double price;
    int quantity;
};

int showMenu();
void createFile(std::fstream&);
void displayFile(std::fstream&);
void displayRecord(std::fstream&, int);
void modifyRecord(std::fstream&);

int main()
{
    std::fstream inventoryFile;

    createFile(inventoryFile);

    int choice;
    do
    {
        choice = showMenu();

        //This takes the users input and then uses the corrisponding function
        switch (choice)
        {
        case 1:
            displayFile(inventoryFile);
            break;
        case 2:
            int recordNum;
            std::cout << "Enter the record number: ";
            std::cin >> recordNum;
            displayRecord(inventoryFile, recordNum);
            break;
        case 3:
            modifyRecord(inventoryFile);
            break;
        case 4:
            std::cout <<  "Exiting the program.\n";
            break;
        default:
            std::cout << "Invalid choice.\n";
        }

    } while (choice != 4);

    inventoryFile.close();

    return 0;
}

// This funciton shows the menu options and then takes the users choice
int showMenu()
{
    int choice;

    std::cout << "\nMenu Options:\n";
    std::cout << "1. Display the entire inventory\n";
    std::cout << "2. Display a particular product\n";
    std::cout << "3. Modify a product\n";
    std::cout << "4. Exit the program\n";
    std::cout << "Enter your choice: ";
    std::cin >> choice;

    return choice;
}

// This creates a file to store the data and then puts the sample data into it.
void createFile(std::fstream& file)
{
    file.open("inventory.dat", std::ios::out | std::ios::in | std::ios::binary);

    if (!file)
    {
        file.open("inventory.dat", std::ios::out | std::ios::binary);
        file.close();
        file.open("inventory.dat", std::ios::out | std::ios::in | std::ios::binary);
    }

    Product sampleData[3] = {
        {101, "Product 1", 15.99, 50},
        {102, "Product 2", 24.99, 75},
        {103, "Product 3", 39.99, 100} };

    for (int i = 0; i < 3; i++)
    {
        file.write(reinterpret_cast<char*>(&sampleData[i]), sizeof(Product));
    }
}

// This displays the products stored on the file in a semi visually apealing way.
void displayFile(std::fstream& file)
{
    file.clear();
    file.seekg(0, std::ios::beg);

    Product tempProduct;
    int recordNumber = 0;

    std::cout << "\nDisplaying the entire inventory:\n";

    while (file.read(reinterpret_cast<char*>(&tempProduct), sizeof(Product)))
    {
        std::cout << "Record Number: " << recordNumber++ << std::endl;
        std::cout << "Number: " << tempProduct.number << std::endl;
        std::cout << "Name: " << tempProduct.name << std::endl;
        std::cout << "Price: $" << tempProduct.price << std::endl;
        std::cout << "Quantity: " << tempProduct.quantity << std::endl;
        std::cout << "------------------------------------\n";
    }
}

// This displays one record that the user inputs into the program
void displayRecord(std::fstream& file, int recordNumber)
{
    file.clear();
    file.seekg(recordNumber * sizeof(Product), std::ios::beg);

    Product tempProduct;

    if (file.read(reinterpret_cast<char*>(&tempProduct), sizeof(Product)))
    {
        std::cout << "\nDisplaying record number: " << recordNumber << std::endl;
        std::cout << "Number: " << tempProduct.number << std::endl;
        std::cout << "Name: " <<  tempProduct.name << std::endl;
        std::cout << "Price: $" << tempProduct.price << std::endl;
        std::cout << "Quantity: " << tempProduct.quantity << std::endl;
        std::cout << "------------------------------------\n";
    }
    else
    {
        std::cout << "\nError: Record number " << recordNumber << " not found.\n";
    }
}

// This modifies a chosen record from the file
void modifyRecord(std::fstream& file)
{
    int recordNumber;
    std::cout << "Enter the record number to modify: ";
    std::cin >> recordNumber;

    file.clear();
    file.seekg(recordNumber * sizeof(Product), std::ios::beg);

    Product tempProduct;

    if (file.read(reinterpret_cast<char*>(&tempProduct), sizeof(Product)))
    {
        std::cout << "\nCurrent product details:\n";
        std::cout << "Number: " << tempProduct.number << std::endl;
        std::cout << "Name: " << tempProduct.name << std::endl;
        std::cout << "Price: $" << tempProduct.price << std::endl;
        std::cout << "Quantity: " << tempProduct.quantity << std::endl;

        std::cout << "\nEnter new product details:\n";
        std::cout << "Number: ";
        std::cin >> tempProduct.number;
        std::cin.ignore();
        std::cout << "Name: ";
        std::cin.getline(tempProduct.name, NAME_SIZE);
        std::cout << "Price: $";
        std::cin >> tempProduct.price;
        std::cout << "Quantity: ";
        std::cin >> tempProduct.quantity;

        file.seekp(recordNumber * sizeof(Product), std::ios::beg);
        file.write(reinterpret_cast<char*>(&tempProduct), sizeof(Product));

        std::cout << "Record number " << recordNumber << " has been updated.\n";
    }
    else
    {
        std::cout << "\nError: Record number " << recordNumber << " not found. \n";
    }
}