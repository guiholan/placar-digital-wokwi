# placar-digital-wokwi

Placar de jogo com Arduino Uno: pontos e faltas de cada time, cronômetro e contagem de sets/períodos.

São 11 displays de 7 segmentos, cada um atrás de um registrador 74HC595 em cascata, então o Arduino controla todos com só 3 pinos (dado, clock e latch). Os botões somam ponto e falta para cada time e avançam o período.

Simulação: https://wokwi.com/projects/431584231150471169
