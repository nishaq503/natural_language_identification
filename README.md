# <center> Natural Language Detection </center>

Welcome to the final project for CSC211. Please read this document thoroughly before starting on your project.

Your task is to implement a machine learning algorithm that detects what language a given text file is written in. You will do this by employing a method called **n-gram frequency analysis**. We will restrict our analysis to languages that use the english alphabet.

## Machine Learning

When we use statistics in computer science, this is called **machine learning**. It is a beautiful field yet in its relative infancy. Using the computational power at our fingertips, we use machine learning (a.k.a. statistical models) to perform a variety of tasks. These tasks range from predicting disease spread in a population to finding out which ads to present to you to best catch your attention. Voice recognition software like Siri and Cortana use machine learning to better understand what people are saying. We can use machine learning to look and pictures of someone's face and guess their age with remarkable accuracy. The variety of tasks that can be tackled with this approach is limited only by your imagination.

Any machine learning project is implemented  with a 2-step process. The first step is to **train** an algorithm on a provided sample of data. In our case, you will be provided with some text files in various languages. Your algorithm will train on those files. The second step is to test your **trained algorithm** on some files that contain text different from that in the training files. Done correctly, your algorithm will predict, with remarkable accuracy, the language that the test file is written in.

## n-gram Frequency Analysis

An **n-gram** is a sequence of n consecutive characters from a given sample of text. For this project, you will take **n = 3**. From here on out, we will call a sequence of 3 consecutive characters a **trigram**.

The idea behind **trigram frequency analysis** is that you measure the **frequency** of each trigram in a given text file. You will count how many times each trigram occurs in a given text file and you will store these frequencies in a vector. We will call such vectors **frequency vectors**.