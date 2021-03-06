package com.example.jsonparsingcomparison;

final class Data
{
    private static final String Literal = "{\"_id\":\"570c8b4efc7a942634a8d628\",\"index\":0,\"guid\":\"c4e170d3 - 1866 - 424e-879f - 2789b21ff44c\",\"isActive\":false,\"balance\":1289.89,\"picture\":\"http://placehold.it/32x32\",\"age\":24,\"name\":\"Iris Kidd\",\"gender\":\"female\",\"company\":\"SNACKTION\",\"email\":\"iriskidd@snacktion.com\",\"phone\":\"+1 (849) 517-2852\",\"address\":\"338 Rutland Road, Keyport, South Carolina, 1263\",\"about\":\"Esse commodo amet aliquip minim anim do ipsum sunt. Nisi cupidatat enim ad velit est anim elit laborum officia aliquip proident cupidatat. Nulla sit labore dolore tempor ut irure dolore irure labore. Adipisicing dolor sit cupidatat fugiat et ea ullamco aliquip consequat amet labore est adipisicing in.\",\"latitude\":71.401699,\"longitude\":102.949369,\"tags\":[\"fugiat\",\"in\",\"dolore\",\"id\",\"anim\",\"veniam\",\"cupidatat\"],\"greeting\":\"Hello, Iris Kidd! You have 2 unread messages.\"}";

    public static String getBigJson()
    {
        StringBuilder stringBuilder = new StringBuilder();

        stringBuilder.append('[');
        for (int i = 0; i < 9999; i++)
        {
            stringBuilder.append(Literal)
                         .append(',');
        }
        stringBuilder.append(Literal);
        stringBuilder.append(']');

        return stringBuilder.toString();
    }

    public static String getSmallJson()
    {
        return Literal;
    }
}
