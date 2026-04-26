$(function() {
    $('.show-problem').click(function(e) {
        e.preventDefault();
        $(this).text('로딩 중').removeClass('show-problem')
        var _this = $(this);
        setTimeout(function() {
            _this.parent().next().show();
            _this.parent().remove();
        }, 0);
    });
    $('blockquote .tab-v2 li a').click(function(e) {
        if ($(this).parent().hasClass('active')) e.preventDefault();
    });
});

