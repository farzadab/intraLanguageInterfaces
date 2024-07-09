'''
Example usage for process_data for testing the data types
more examples on module/test_mathUtils.py

def process_data(a, b, c, d, e):
    print(f"Integer: {a}")
    print(f"Float: {b}")
    print(f"String: {c}")
    print(f"List: {d}")
    print(f"Dictionary: {e}")
    return a + int(b)  # Just a simple return for demonstration


if __name__ == "__main__":
    result = process_data(1, 2.5, "hello", [1, 2, 3], {"key": "value"})
    print(f"Result: {result}")