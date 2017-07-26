#ifndef LINKEDLIST_H
#define	LINKEDLIST_H

#include <iostream>
#include "NodeList.h"

template <class Element>

class LinkedList {
    
    public:
        // Constructors and Destructor.
        LinkedList() {

            this->first = 0;
            this->last = 0;
            this->size = 0;
        }

        virtual ~LinkedList() {

            this->clear();
        }

        // Getters and Setters.
        unsigned getSize() { 

            return this->size;
        }

        // Instance Members.
        void addFirst(Element element) {

            NodeList<Element>* newElement = new NodeList<Element>(element, 0, this->first);

            if (this->first) {

                this->first->setPrevious(newElement);
            } else {

                this->last = newElement;
            }

            this->first = newElement;

            this->size++;
        }

        void addFirst(Element* element) {

            NodeList<Element>* newElement = new NodeList<Element>(element, 0, this->first);

            if (this->first) {

                this->first->setPrevious(newElement);
            } else {

                this->last = newElement;
            }

            this->first = newElement;

            this->size++;
        }

        void addLast(Element element) {

            NodeList<Element>* node = new NodeList<Element>(element, this->last, 0);

            if (this->last) {

                this->last->setNext(node);
            } else {

                this->first = node;
            }

            this->last = node;

            this->size++;
        }

        void addLast(Element* element) {

            NodeList<Element>* node = new NodeList<Element>(element, this->last, 0);

            if (this->last) {

                this->last->setNext(node);
            } else {

                this->first = node;
            }

            this->last = node;

            this->size++;
        }

        Element* getFirst() {

            if (this->first) {

                return this->first->getData();
            }

            return 0;
        }

        Element* getLast() {

            if (this->last) {

                return this->last->getData();
            }

            return 0;
        }

        Element* get(unsigned index) {

            if (index >= this->size) {

                return 0;
            }

            NodeList<Element>* aux = this->first;

            for (unsigned i = 0 ; i < index ; i++, aux = aux->getNext());

            return aux->getData();
        }

        Element* get(Element element) {

            NodeList<Element>* aux = this->first;

            for (unsigned i = 0 ; i < this->size ; i++, aux = aux->getNext()) {

                if ((*(aux->getData())) == element) {

                    return aux->getData();
                }
            }

            return 0;
        }

        Element* get(Element* element) {

            NodeList<Element>* aux = this->first;

            for (unsigned i = 0 ; i < this->size ; i++, aux = aux->getNext()) {

                if ((*(aux->getData())) == (*element)) {

                    return aux->getData();
                }
            }

            return 0;
        }
        
        Element* begin(){
        	if(this->size <= 0){
        		return NULL;
        	}
        	this->target = this->first;
	       	return this->target->getData();
        }
        
        Element* getNext(){
        	if(target != last){
        		target = target->getNext();
        		return target->getData();
        	} else {
        		return NULL;
        	}
        }
        
        bool isEnd(){
        	return(target == last);
        }
        
        Element* remove(Element* element) {

            NodeList<Element>* aux = this->first;

            for (unsigned i = 0 ; i < this->size ; i++, aux = aux->getNext()) {

                if (aux->getData() == element) {
					//
				    if ( this->first->getData() == aux->getData() )
				    {
						removeFirst();
				    }
				    else if( this->last->getData() == aux->getData() )
				    {
				    	removeLast();
				    }
					else
					{	
						this->size--;
						
						(aux)->getNext()->setPrevious((aux)->getPrevious());
						(aux)->getPrevious()->setNext((aux)->getNext());
                    
                    	delete aux;
                    }
                    
                    return aux->getData();
                }
            }
            
            return NULL;
        }

        Element* removeFirst() {

            if (!this->size) {

                return 0;
            }

            NodeList<Element>* node = this->first;
            if (this->first->getNext()) {

                this->first->getNext()->setPrevious(0);
            } else {

                this->last = 0;
            }

            this->size--;
            if (this->first) {
                
                this->first = this->first->getNext();
            }

            Element* data = node->getData();

            delete node;

            return data;
        }

        Element* removeLast() {

            if (!this->size) {

                return 0;
            }

            NodeList<Element>* node = this->last;
            if (this->last) {

                if (this->last->getPrevious()) {

                    this->last->getPrevious()->setNext(0);
                } else {

                    this->first = 0;
                }
            }

            this->size--;
            if (this->last) {
                
                this->last = this->last->getPrevious();
            }

            Element* data = node->getData();

            delete node;

            return data;
        }

        void clear() {

            while (this->size) {

                delete removeFirst();
            }
        }

    protected:
        // Instance Members.
        NodeList<Element>* first;
        NodeList<Element>* last;
        NodeList<Element>* target;
        unsigned size;
    
};

#endif
