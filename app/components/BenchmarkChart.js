// BenchmarkChart.js
// from https://www.d3-graph-gallery.com/graph/scatter_basic.html
// and https://betterprogramming.pub/react-d3-plotting-a-line-chart-with-tooltips-ed41a4c31f4f

import React, { useEffect } from 'react'
import * as d3 from 'd3'

function BenchmarkChart (props) {
  const { data, width, height, xLabel, yLabel, xType, yType, isLowerBetter } = props

  React.useEffect(() => {
    function drawChart () {
      d3.select('#qrack-line-chart-container')
        .select('svg')
        .remove()
      d3.select('#qrack-line-chart-container')
        .select('.tooltip')
        .remove()

      const margin = { top: 20, right: 256, bottom: 60, left: 272 }
      let lWidth = width - margin.left - margin.right
      const lHeight = height - margin.top - margin.bottom
      if (lWidth < 300) {
        const shim = (width / 2) - 158
        margin.right = shim
        margin.left = shim + 16
        lWidth = 300
      }
      const yMinValue = d3.min(data, d => d.value)
      const yMaxValue = d3.max(data, d => d.value)
      const xMinValue = d3.min(data, d => d.label)
      const xMaxValue = d3.max(data, d => d.label)

      // set the dimensions and margins of the graph

      // append the svg object to the body of the page
      const svg = d3.select('#qrack-line-chart-container')
        .append('svg')
        .attr('width', width)
        .attr('height', height)
        .append('g')
        .attr('transform',
          'translate(' + margin.left + ',' + margin.top + ')')

      // Add X axis
      const x = (xType === 'time')
        ? d3.scaleTime()
          .domain([xMinValue, xMaxValue])
          .range([0, lWidth])
        : d3.scaleLinear()
          .domain([xMinValue, xMaxValue])
          .range([0, lWidth])

      svg.append('g')
        .attr('transform', 'translate(0,' + lHeight + ')')
        .call(d3.axisBottom(x).tickSizeOuter(0))

      const yDomain = [(yMinValue < 0) ? yMinValue : 0, (yMaxValue < 0) ? 0 : yMaxValue]

      // Add Y axis
      const y = (yType === 'time')
        ? d3.scaleTime()
          .domain(yDomain)
          .range([lHeight, 0])
        : d3.scaleLinear()
          .domain(yDomain)
          .range([lHeight, 0])

      const yTickAxis = d3.axisLeft(y)
        .tickSize(0)
        .tickSizeInner(-lWidth)
        .tickSizeOuter(0)
        .tickFormat(d3.format('.1e'))

      svg.append('g')
        .attr('class', 'grid')
        .call(yTickAxis)

      // Add dots
      const dots = svg.append('g')
        .selectAll('dot')
        .data(data)
        .enter()

      dots.append('circle')
        .attr('cx', function (d) { return x(d.label) })
        .attr('cy', function (d) { return y(d.value) })
        .attr('r', 3.0)
        .style('fill', '#69b3a2')

      dots.append('text').text(function (d) { return d.method })
        .attr('x', function (d) { return x(d.label) })
        .attr('y', function (d) { return y(d.value) })

      const line = d3.line()
        .x(function (d) { return x(d.label) })
        .y(function (d) { return y(d.value) })

      svg.append('path')
        .datum(data)
        .attr('fill', 'none')
        .attr('stroke-width', 1.5)
        .attr('stroke', 'steelblue')
        .attr('d', line)

      const xAxisLabelX = lWidth / 2
      const xAxisLabelY = lHeight + 40

      svg.append('text')
        .attr('class', 'x label')
        .attr('text-anchor', 'middle')
        .attr('x', xAxisLabelX)
        .attr('y', xAxisLabelY)
        .text(xLabel)

      const yAxisLabelX = -60
      const yAxisLabelY = lHeight / 2

      svg.append('g')
        .attr('transform', 'translate(' + yAxisLabelX + ', ' + yAxisLabelY + ')')
        .append('text')
        .attr('text-anchor', 'middle')
        .attr('transform', 'rotate(-90)')
        .text(yLabel)
    }
    drawChart()
  }, [data, width, height, xLabel, yLabel, xType, yType, isLowerBetter])

  return <div id='qrack-line-chart-container' />
}

export default BenchmarkChart
