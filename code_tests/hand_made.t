#this is made by me prompting chatgpt and us debuging together.

# Step 1: Initialize the left of the tape to N ones
Write1_L1,0: 1 L Write1_L2  # Write 1 and move left
Write1_L1,1: 1 L Write1_L2  # Write 1 and move left

Write1_L2,0: 1 L Write1_L3  # Write 1 and move left
Write1_L2,1: 1 L Write1_L3  # Write 1 and move left

Write1_L3,0: 1 L Write1_L4  # Write 1 and move left
Write1_L3,1: 1 L Write1_L4  # Write 1 and move left

Write1_L4,0: 1 L Write1_L5  # Write 1 and move left
Write1_L4,1: 1 L Write1_L5  # Write 1 and move left

Write1_L5,0: 1 L Return_R1  # Write 1 and move left
Write1_L5,1: 1 L Return_R1  # Write 1 and move left

# Return to the start of the tape
Return_R1,1: 1 R Return_R2  # Move right
Return_R1,0: 0 R Return_R2  # Move right

Return_R2,1: 1 R Return_R3  # Move right
Return_R2,0: 0 R Return_R3  # Move right

Return_R3,1: 1 R Return_R4  # Move right
Return_R3,0: 0 R Return_R4  # Move right

Return_R4,1: 1 R Return_R5  # Move right
Return_R4,0: 0 R Return_R5  # Move right

Return_R5,1: 1 R Initialize_Right  # Move right
Return_R5,0: 0 R Initialize_Right  # Move right

Initialize_Right,0: 0 R Start_Copy  # Move right to start position
Initialize_Right,1: 1 R Start_Copy

# Step 2: Copy N ones to the right of the tape
Start_Copy,0: 1 R Decrement_Left  # Start of right tape
Start_Copy,1: 1 R Decrement_Left

Decrement_Left,1: 0 L Move_Left  # Decrement counter and move left
Decrement_Left,0: 0 L Halt_Check  # If no more ones on the left, move to Halt_Check

Halt_Check,1: 1 L Move_Left  # If it's a 1, continue moving left
Halt_Check,0: 0 S halt  # If it's a 0, halt

Move_Left,1: 1 L Move_Left  # Move left to start of tape
Move_Left,0: 0 R Find_Right_Tape  # Found start, move right

Find_Right_Tape,1: 1 R Find_Right_Tape  # Skip ones to the right
Find_Right_Tape,0: 0 R Write_One  # Found 0, move right

Write_One,1: 1 R Write_One  # Skip ones
Write_One,0: 1 L Return_Left  # Set 0 to 1, move left

Return_Left,1: 1 L Return_Left  # Move left to start
Return_Left,0: 0 L Decrement_Left  # Repeat loop
