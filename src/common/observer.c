#include "observer.h"
#include <stdio.h>
#include <stdlib.h>
#define T Observer
#define S Subject

void __subject_attach(S* subject, T* observer);
void __subject_detach(S* subject, T* observer);
void __subject_notify(S* subject);
void __subject_setState(S* subject, void* newState);
void subject_destructor(S* subject);

T* observer_constructor(Observer_update update){
    T* observer = (T*)malloc(sizeof(T));
    if (observer != NULL) {
        observer->update = update;
    }
    return observer;
}

S* subject_constructor(void* initialState) {
    S* subject = (Subject*)malloc(sizeof(Subject));
    if (subject != NULL) {
        subject->state = initialState;
        subject->numObservers = 0;
        subject->observers = NULL;
        subject->attach = __subject_attach;
        subject->detach = __subject_detach;
        subject->notify = __subject_notify;
        subject->set_state = __subject_setState;
        subject->destructor = subject_destructor;
    }
    return subject;
}

void __subject_attach(S* subject, T* observer) {
    subject->numObservers++;
    subject->observers = (T**)realloc(subject->observers, subject->numObservers * sizeof(T*));
    subject->observers[subject->numObservers - 1] = observer;
}

void __subject_detach(S* subject, T* observer) {
    int i, j = 0;
    T** newObservers = (T**)malloc((subject->numObservers - 1) * sizeof(T*));

    for (i = 0; i < subject->numObservers; i++) {
        if (subject->observers[i] != observer) {
            newObservers[j++] = subject->observers[i];
        }
    }

    free(subject->observers);
    subject->observers = newObservers;
    subject->numObservers--;
}

void __subject_notify(S* subject) {
    for (int i = 0; i < subject->numObservers; i++) {
        subject->observers[i]->update(subject->state);
    }
}

void __subject_setState(S* subject, void* newState) {
    subject->state = newState;
    subject->notify(subject);
}

void subject_destructor(S* subject) {
    if (subject != NULL) {
        free(subject);
    }
}


#undef T
#undef S
