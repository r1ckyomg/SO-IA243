package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
	"time"
)

type Question struct {
	Text          string
	AcceptAnswers []string
	Hints         []string
}

var (
	prepDuration  = 3 * time.Second
	roundDuration = 15 * time.Second
	hintInterval  = 3 * time.Second
)

func main() {
	fmt.Println("квиз. 'exit' - выход.")

	questions := []Question{
		{
			Text:          "Какой примитив в Go используется для передачи данных между горутинами?",
			AcceptAnswers: []string{"канал", "chan", "channel"},
			Hints: []string{
				"оператор <-",
				"бывает буферизированным",
				"начинается на 'к' / 'c'",
			},
		},
		{
			Text:          "Встроенная хеш-таблица в Go?",
			AcceptAnswers: []string{"map", "мапа", "хэш-таблица", "хеш-таблица", "хэшмапа", "хешмапа", "словарь"},
			Hints: []string{
				"make(map[...])",
				"3 буквы",
			},
		},
		{
			Text:          "Ключевое слово для отложенного вызова функции?",
			AcceptAnswers: []string{"defer", "дефер"},
			Hints: []string{
				"для Close/Unlock",
				"LIFO",
				"5 букв на 'd'",
			},
		},
	}

}

func isCorrect(input string, accepted []string) bool {
	s := strings.ToLower(strings.TrimSpace(input))
	for _, a := range accepted {
		if s == strings.ToLower(a) {
			return true
		}
	}
	return false
}

func startInputReader() <-chan string {
	ch := make(chan string)
	go func() {
		scanner := bufio.NewScanner(os.Stdin)
		for scanner.Scan() {
			ch <- scanner.Text()
		}
	}()
	return ch
}

func drainInput(ch <-chan string) {
	for {
		select {
		case <-ch:
		default:
			return
		}
	}
}
