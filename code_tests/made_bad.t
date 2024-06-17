#this is hand_made.t plus code that can be optimized away

# Step 1: Initialize the left of the tape to N ones
Write1_L1,0: 1 L NoOp1  # Write 1 and move left
Write1_L1,1: 1 L NoOp1  # Write 1 and move left

NoOp1,0: 0 S Write1_L2  # Do nothing
NoOp1,1: 1 S Write1_L2  # Do nothing

Write1_L2,0: 1 L FlipBack1  # Write 1 and move left
Write1_L2,1: 1 L FlipBack1  # Write 1 and move left

FlipBack1,0: 1 S FlipAgain1
FlipBack1,1: 0 S Write1_L3
FlipAgain1,0: 0 S Write1_L3
FlipAgain1,1: 1 S Write1_L3

Write1_L3,0: 1 L NoOp2  # Write 1 and move left
Write1_L3,1: 1 L NoOp2  # Write 1 and move left

NoOp2,0: 0 S Write1_L4  # Do nothing
NoOp2,1: 1 S Write1_L4  # Do nothing

Write1_L4,0: 1 L MoveBack1  # Write 1 and move left
Write1_L4,1: 1 L MoveBack1  # Write 1 and move left

MoveBack1,0: 0 L MoveForward1
MoveBack1,1: 1 L MoveForward1
MoveForward1,0: 0 R Write1_L5
MoveForward1,1: 1 R Write1_L5

Write1_L5,0: 1 L Return_R1  # Write 1 and move left
Write1_L5,1: 1 L Return_R1  # Write 1 and move left

# Return to the start of the tape
Return_R1,1: 1 R NoOp3  # Move right
Return_R1,0: 0 R NoOp3  # Move right

NoOp3,0: 0 S Return_R2  # Do nothing
NoOp3,1: 1 S Return_R2  # Do nothing

Return_R2,1: 1 R FlipBack2  # Move right
Return_R2,0: 0 R FlipBack2  # Move right

FlipBack2,0: 1 S FlipAgain2
FlipBack2,1: 0 S Return_R3
FlipAgain2,0: 0 S Return_R3
FlipAgain2,1: 1 S Return_R3

Return_R3,1: 1 R MoveBack2  # Move right
Return_R3,0: 0 R MoveBack2  # Move right

MoveBack2,0: 0 L MoveForward2
MoveBack2,1: 1 L MoveForward2
MoveForward2,0: 0 R Return_R4
MoveForward2,1: 1 R Return_R4

Return_R4,1: 1 R NoOp4  # Move right
Return_R4,0: 0 R NoOp4  # Move right

NoOp4,0: 0 S Return_R5  # Do nothing
NoOp4,1: 1 S Return_R5  # Do nothing

Return_R5,1: 1 R Initialize_Right  # Move right
Return_R5,0: 0 R Initialize_Right  # Move right

Initialize_Right,0: 0 R Start_Copy  # Move right to start position
Initialize_Right,1: 1 R Start_Copy

# Step 2: Copy N ones to the right of the tape
Start_Copy,0: 1 R FlipBack3  # Start of right tape
Start_Copy,1: 1 R FlipBack3

FlipBack3,0: 1 S FlipAgain3
FlipBack3,1: 0 S Decrement_Left
FlipAgain3,0: 0 S Decrement_Left
FlipAgain3,1: 1 S Decrement_Left

Decrement_Left,1: 0 L NoOp5  # Decrement counter and move left
Decrement_Left,0: 0 L Halt_Check  # If no more ones on the left, move to Halt_Check

NoOp5,0: 0 S Move_Left  # Do nothing
NoOp5,1: 1 S Move_Left  # Do nothing

Halt_Check,1: 1 L Move_Left  # If it's a 1, continue moving left
Halt_Check,0: 0 S halt  # If it's a 0, halt

Move_Left,1: 1 L Move_Left  # Move left to start of tape
Move_Left,0: 0 R NoOp6  # Found start, move right

NoOp6,0: 0 S Find_Right_Tape  # Do nothing
NoOp6,1: 1 S Find_Right_Tape

Find_Right_Tape,1: 1 R MoveBack3  # Skip ones to the right
Find_Right_Tape,0: 0 R Write_One  # Found 0, move right

MoveBack3,0: 0 L MoveForward3
MoveBack3,1: 1 L MoveForward3
MoveForward3,0: 0 R Write_One
MoveForward3,1: 1 R Write_One

Write_One,1: 1 R FlipBack4  # Skip ones
Write_One,0: 1 L Return_Left  # Set 0 to 1, move left

FlipBack4,0: 1 S FlipAgain4
FlipBack4,1: 0 S Write_One
FlipAgain4,0: 0 S Write_One
FlipAgain4,1: 1 S Write_One

Return_Left,1: 1 L Return_Left  # Move left to start
Return_Left,0: 0 L Decrement_Left  # Repeat loop
