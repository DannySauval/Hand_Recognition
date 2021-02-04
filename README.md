<!-- PROJECT LOGO -->
<br />
<p align="center">
  <h3 align="center">Hand Recognition</h3>

  <p align="center">
    Track and interpret hand shape using OpenCV and C++
    <br />
    <a href="https://github.com/DannySauval/Hand_Recognition"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://youtu.be/MyM8FlyXM8k">View Demo on YouTube</a>
    ·
    <a href="https://github.com/DannySauval/Hand_Recognition/issues">Report Bug</a>
    ·
    <a href="https://github.com/DannySauval/Hand_Recognition/issues">Request Feature</a>
  </p>
</p>

<!-- TABLE OF CONTENTS -->
<details open="open">
  <summary><h2 style="display: inline-block">Table of Contents</h2></summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgements">Acknowledgements</a></li>
  </ol>
</details>

<!-- ABOUT THE PROJECT -->

## About The Project

Back in 2017, I was taking a C++ class at Polytech Sorbonne. For the final project, we could choose to program whatever we wanted to, the only rule was to use C++ (of course), OpenCV and a camera.

I have always been interested about sign language, so I wanted to learn more about it. I learned a lot about sign language, and about what kinds of computer vision solutions existed to be able to understand and transcript or speak out loud the signs.

At the time of the project, I didn't find a lot of projects addressing the problem, so I decided to experiment on my own. I could play with my webcam and what I knew about computer vision at the time to be able to get data from the signs I was doing with my hand.

The level of detection is simple, it can actually detect the number of raised fingers, which is still a good start. I learned a lot about what could be done.

List of detectable features :

- High Five
- Four fingers
- Three fingers
- Metal Sign (actually two fingers)
- Every other sign is set to "unknown"

If I had to go deeper into this project, I am pretty sure I would do this differently, like use what I know about AI. I could create an algorithm that would help me create a database (if one does not exist already) and use a CNN to get classifiable data.

<!-- GETTING STARTED -->

## Getting Started

To get a local copy up and running follow these simple steps.

### Prerequisites

There are a few requirements to be able to run the demo.

- A C++ compiler, I let you Google this !
- OpenCV 2 : https://opencv.org/
- SFML library :

```sh
sudo apt-get install libsfml-dev
```

### Installation

1. Clone the repo
   ```sh
   git clone https://github.com/DannySauval/Hand_Recognition
   ```
2. Run make
   ```sh
   make
   ```

<!-- USAGE EXAMPLES -->

## Usage

You can watch my YouTube video to get an idea of what you can do with the project ! YouTube video : https://youtu.be/MyM8FlyXM8k

<!-- CONTACT -->

## Contact

Danny SAUVAL

- Twitter : [@DSauval](https://twitter.com/DSauval)
- Youtube : [DannySauval](https://www.youtube.com/channel/UC_pCdmt0cSF49st1b5q6yIQ)
- Email : sauvaldanny@gmail.com

Project Link: [https://github.com/DannySauval/Hand_Recognition](https://github.com/DannySauval/Hand_Recognition)

<!-- ACKNOWLEDGEMENTS -->

## Acknowledgements

- Ludovic Saint-Bauzel for beeing a nice and useful teacher !
- Internet !
