import numpy as np
import pandas as pd

# Constants
NUM_STUDENTS = 348
MEAN_GRADE = 70
STD_DEV_GRADE = 10

# Generate random student data
first_names = [f"FirstName{i}" for i in range(1, NUM_STUDENTS + 1)]
last_names = [f"LastName{i}" for i in range(1, NUM_STUDENTS + 1)]
grades = np.random.normal(MEAN_GRADE, STD_DEV_GRADE, NUM_STUDENTS).clip(0, 100)

# Create a DataFrame
students = pd.DataFrame({
    'First Name': first_names,
    'Last Name': last_names,
    'Grade': grades
})

# Save to a CSV file
students.to_csv('students.csv', index=False)
