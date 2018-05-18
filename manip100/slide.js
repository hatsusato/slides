var slideshow = remark.create({
    highlightLines: true,
    highlightStyle: 'foundation',
});
const color_sum = 512;
slideshow.on('beforeShowSlide', (slide) => {
    const index = slide.getSlideIndex();
    const container = document.querySelectorAll('.remark-slide-container')[index];
    const content = container.querySelector('.remark-slide-content');
    const rgb_ratio = [Math.random(), Math.random(), Math.random()];
    const max_color = Math.max(...rgb_ratio);
    const min_color = Math.min(...rgb_ratio);
    const max_index = rgb_ratio.indexOf(max_color);
    const min_index = rgb_ratio.indexOf(min_color);
    rgb_ratio[max_index] = rgb_ratio[max_index] * 2.0;
    rgb_ratio[min_index] = rgb_ratio[min_index] * 0.5;
    const sum_ratio = rgb_ratio[0] + rgb_ratio[1] + rgb_ratio[2];
    const red = parseInt(Math.min(255, color_sum * rgb_ratio[0] / sum_ratio));
    const green = parseInt(Math.min(255, color_sum * rgb_ratio[1] / sum_ratio));
    const blue = parseInt(Math.min(255, color_sum * rgb_ratio[2] / sum_ratio));
    if (content.style.backgroundColor === '') {
        container.querySelector('.remark-slide-content').style.backgroundColor = `rgb(${red}, ${green}, ${blue})`;
    }
});
