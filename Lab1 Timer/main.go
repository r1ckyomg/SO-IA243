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
	hintInterval  = 5 * time.Second
)

func main() {
	fmt.Println("квизфвфаы. 'exit' - выход.")

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

	inputChan := startInputReader()
	totalScore := 0

	for i, q := range questions {
		score, quit := playRound(i+1, q, inputChan)
		if quit {
			fmt.Println("выход")
			return
		}
		totalScore += score
	}

	fmt.Printf("\nитог: %d\n", totalScore)

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

func playRound(roundNum int, q Question, inputChan <-chan string) (score int, quit bool) {
	drainInput(inputChan)

	fmt.Printf("\nраунд %d (через %v)...\n", roundNum, prepDuration)
	prepTimer := time.NewTimer(prepDuration)
	<-prepTimer.C

	fmt.Println(q.Text)
	fmt.Print("> ")

	roundTimer := time.NewTimer(roundDuration)
	defer roundTimer.Stop()

	hintTicker := time.NewTicker(hintInterval)
	defer hintTicker.Stop()

	startTime := time.Now()
	hintIdx := 0

	for {
		select {
		case <-roundTimer.C:
			fmt.Printf("\nвремя вышло. ответ: %s\n", q.AcceptAnswers[0])
			return 0, false

		case <-hintTicker.C:
			if hintIdx < len(q.Hints) {
				fmt.Printf("\n[подсказка]: %s\n> ", q.Hints[hintIdx])
				hintIdx++
			} else {
				left := roundDuration - time.Since(startTime)
				if left > 0 {
					fmt.Printf("\n[осталось %v]\n> ", left.Round(time.Second))
				}
			}

		case ans := <-inputChan:
			trimmed := strings.TrimSpace(ans)
			if strings.EqualFold(trimmed, "exit") || strings.EqualFold(trimmed, "quit") {
				return 0, true
			}

			if isCorrect(trimmed, q.AcceptAnswers) {
				roundTimer.Stop()
				hintTicker.Stop()
				earned := 100 - hintIdx*20
				if earned < 20 {
					earned = 20
				}
				fmt.Printf("верно (+%d)\n", earned)
				return earned, false
			}

			fmt.Print("неверно: ")
		}
	}
}
