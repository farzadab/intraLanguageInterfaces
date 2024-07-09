def two_sum(nums, target):
    # Create a dictionary to store the complement and its index
    num_to_index = {}

    # Iterate over the list of numbers
    for index, num in enumerate(nums):
        # Calculate the complement
        complement = target - num

        # Check if the complement is already in the dictionary
        if complement in num_to_index:
            # If found, return the indices of the two numbers
            return [num_to_index[complement], index]

        # Otherwise, add the number and its index to the dictionary
        num_to_index[num] = index

    # If no solution is found, return an empty list
    return []

# Example usage for two_sum
if __name__ == "__main__":
    nums = [2, 7, 11, 15]
    target = 9
    result = two_sum(nums, target)
    print(f"Indices of the two numbers that add up to {target} are: {result}")


def multiply(a,b):
    return a*b



def concat_sum_string(a, b, s):
    try:
        result = int(a) + int(b)
        print(f"{s}: {result}")
        return result
    except ValueError:
        print("Error: Invalid input")
        return None
