def two_sum(nums, target):
    """
    Find two numbers in the list that add up to the target.

    Args:
        nums (list): List of integers.
        target (int): Target sum.

    Returns:
        list: Indices of the two numbers that add up to the target.
    """
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

def multiply(a, b):
    """
    Multiply two numbers.

    Args:
        a (int): First number.
        b (int): Second number.

    Returns:
        int: Product of the two numbers.
    """
    return a * b


def concat_sum_string(a, b, s):
    """
    Concatenate a string with the sum of two numbers.

    Args:
        a (str): First number as a string.
        b (str): Second number as a string.
        s (str): String to concatenate with the sum.

    Returns:
        int: Sum of the two numbers if valid, otherwise None.
    """
    try:
        result = int(a) + int(b)
        print(f"{s}: {result}")
        return result
    except ValueError:
        print("Error: Invalid input")
        return None