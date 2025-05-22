import cv2
import numpy
import time
import random

NumberOfMeasurements = 2
bodiesSize = 5

class Body:
  def __init__(self, mass, position, velocity, acceleration, color):
    self.mass = mass
    self.position = numpy.array(position, dtype=numpy.float64)
    self.velocity = numpy.array(velocity, dtype=numpy.float64)
    self.acceleration = numpy.array(acceleration, dtype=numpy.float64)
    self.color = color
    self.trail = []
    self.max_trail_length = 1000

def solve(bodies):
  distance = {}
  #count distance between bodies
  for i in range(0, (len(bodies) - 1)):
    for j in range((i + 1), len(bodies)):
      for q in range(0, NumberOfMeasurements):
        distance[(i * len(bodies) + j) * NumberOfMeasurements + q] = abs(bodies[i].position[q] - bodies[j].position[q])

  #zeroingAcceleration
  for i in range(0, len(bodies)):
     for q in range(0, NumberOfMeasurements):
        bodies[i].acceleration[q] = 0

  for i in range(0, len(bodies)):
    for j in range(0, len(bodies)):
      if (i != j):
          for q in range(0, NumberOfMeasurements):
            vec = 0
            if (bodies[i].position[q] < bodies[j].position[q]):
              vec = -1
            elif (bodies[i].position[q] > bodies[j].position[q]):
              vec = 1
            if (i < j):
              count = (i * len(bodies) + j) * NumberOfMeasurements + q
            else:
              count = (j * len(bodies) + i) * NumberOfMeasurements + q
            if (distance[count] != 0):
              bodies[i].acceleration[q] += (vec * bodies[j].mass) / (distance[count] ** 2)
    for q in range(0, NumberOfMeasurements):
      bodies[i].velocity[q] += bodies[i].acceleration[q]
      bodies[i].position[q] += bodies[i].velocity[q]
      bodies[i].trail.append((int(bodies[i].position[0]), int(bodies[i].position[1])))
      if len(bodies[i].trail) > bodies[i].max_trail_length:
        bodies[i].trail.pop(0)

#input block
s = int(input("Write duration of video: "))
if (s < 1):
  s = 60
n = int(input("Write number of bodies (from 2 to 400): "))
if ((n < 2) or (n > 400)):
  n = 1 + rand.randrange(399)
fps = int(input("Write number of fotos per second (from 1 to 120): "))
if (fps > 120):
  fps = 120
elif (fps < 1):
  fps = 1
video = cv2.VideoWriter("result.mp4", cv2.VideoWriter_fourcc(*'mp4v'), fps, (1920, 1080))

#bodies initialization
bodies = []
colors = []
for i in range(0, n):
  colors.append((random.randrange(255), random.randrange(255), random.randrange(255)))
  mass = 1
  coord = []
  for j in range(0, NumberOfMeasurements):
    coord.append(random.randrange(1000))
  speed = []
  for j in range(0, NumberOfMeasurements):
    speed.append(1 - random.randrange(2))
  bodies.append(Body(mass, coord, speed, numpy.zeros(NumberOfMeasurements), colors[i]))

#main programm
for i in range(0, (s * fps)):
  image = numpy.zeros((1080, 1920, 3), dtype = numpy.uint8)
  for body in bodies:
    for j in range(1, len(body.trail)):
      alpha = j / len(body.trail)
      color = tuple(int(c * alpha) for c in body.color)
      cv2.line(image, body.trail[j - 1], body.trail[j], color, 1)

    for j in range(0, n):
        cv2.circle(image, (int(bodies[j].position[0]), int(bodies[j].position[1])), bodiesSize, bodies[j].color, -1)
        if (bodies[j].position[0] > 1920):
          bodies[j].position[0] = 0
        elif (bodies[j].position[0] < 0):
          bodies[j].position[0] = 1920
        if (bodies[j].position[1] > 1080):
          bodies[j].position[1] = 0
        elif(bodies[j].position[1] < 0):
           bodies[j].position[1] = 1080

  solve(bodies)
  video.write(image)
  print(int((i * 100) / (s * fps)), "% is done", end='\r')

cv2.destroyAllWindows()
video.release()
