# TP QT Fitts law visualization
TP on C++ Qt for creating a Fitts law application

## Choices

We chose to work on the basis of the Project n° 6 given by our professor. It already has a nice interface and some features that we just need to push further.

## Upgrade to make

- [x] Change the button `Annuler` to let the user undersand easily that he can configure his test before starting. 
- [x] Change some things with the saves.
  - [ ] Add of a menu to save the datas of a given test (not save automatically a test).
  - [x] Possibility to visualize the datas (it is impossible for the moment)
    - [ ] :warning:  Do not forget to warn the user that his current datas will be erased and let him the possibility to save.
  - [x] Possibilty to delete a save file
- [x] Make the window more responsive (currently, it is a bit resizeable but not so much).
- [ ] Change some things on the graphs
  - [ ] The variables change their unity depending on which type of law we use (T = log(D/2L) or T = f(D))
  - [ ] The unity on the axis should be easier to read.
  - [x] The datas on the T = f(D) graph are not easy to read, so we must hide them or make them more readable.
  - [ ] Change the datas dynamically when we modify teh variable `a` and `b`
- [x] Add captions on the methods.
- [x] Explain the model [MVC](https://developer.mozilla.org/fr/docs/Glossary/MVC) in the presentation support.

## Additional notes

*This project was realized during the HM40 courses at UTBM* 
