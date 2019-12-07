import subprocess

expected = "19690720"

for noun in range(0,99):
  for verb in range(0,99):
    input = open('input.txt')
    process = subprocess.run (["../build/bin/AOC_2", str(noun), str(verb)], stdin=input, capture_output=True, text=True)
    result = process.stdout
    # print (result)
    if result == expected:
      print ("Correct result: " + result + " Noun: " + str(noun) + " Verb: " + str(verb))
      print ("Puzzle:" + str(100 * noun + verb))
      exit()