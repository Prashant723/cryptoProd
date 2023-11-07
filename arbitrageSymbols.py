# Open and read the first file
with open('modifiedBinanceSpotSymbols.txt', 'r') as file1:
    lines1 = file1.readlines()

# Open and read the second file
with open('ModifiedspotOkexSymbols.txt', 'r') as file2:
    lines2 = file2.readlines()

# Create a new file to store the matching rows side by side
with open('output.txt', 'w') as output_file:
    # Iterate through the lines of both files
    for line1 in lines1:
        column1 = line1.split(",")[1]  # Assuming columns are separated by space
        for line2 in lines2:
            column2 = line2.split(",")[1]  # Assuming columns are separated by space

            # Check if the second column matches
            if column1 == column2:
                # Write the matching rows side by side in the output file
                output_file.write(f'{line1.strip()} , {line2.strip()}\n')

# Print a message to confirm completion
print("Matching rows have been written to 'output.txt'")

