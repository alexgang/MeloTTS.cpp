# Define input and output file paths
input_file = "cmudict_cache.txt"
output_file = "cmudict_cache_order.txt"

# Read and process the file
def process_file(input_file, output_file):
    with open(input_file, "r", encoding="utf-8") as file:
        lines = file.readlines()

    # Define the regular expression for valid keys
    import re
    valid_key_pattern = re.compile(r"^[a-z']+$")

    # Parse and filter key:value pairs
    key_value_pairs = {}
    for line in lines:
        if ":" in line:
            key, value = line.split(":", 1)
            key = key.strip()
            value = value.strip()
            if valid_key_pattern.match(key):
                key_value_pairs[key] = value

    # Sort by key in alphabetical order
    sorted_pairs = sorted(key_value_pairs.items())

    # Output to file
    with open(output_file, "w", encoding="utf-8") as file:
        for key, value in sorted_pairs:
            file.write(f"{key}:{value}\n")

# Execute file processing
process_file(input_file, output_file)
