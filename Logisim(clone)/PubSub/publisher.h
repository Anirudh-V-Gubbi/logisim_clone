#ifndef PUBLISHER_H
#define PUBLISHER_H

#include "subscriber.h"
#include <list>
#include <memory>

template <typename T> class Publisher {
public:
  Publisher(bool enabled = true) : m_enabled(enabled) {}
  virtual ~Publisher() {}

  void Subscribe(std::shared_ptr<Subscriber<T>> subscriber) {
    m_subscribers.push_back(subscriber);
  }

  void Unsubscribe(std::shared_ptr<Subscriber<T>> subscriber) {
    m_subscribers.remove(subscriber);
  }

  void Notify(T &event) {
    if (m_enabled == false)
      return;

    for (auto it = m_subscribers.begin(); it != m_subscribers.end(); it++) {
      (*it)->OnNotify(event);
    }
  }

  void SetEnabled(bool enabled) { m_enabled = enabled; }

  bool GetEnabled() const { return m_enabled; }

private:
  bool m_enabled;
  std::list<std::shared_ptr<Subscriber<T>>> m_subscribers;
};

#endif